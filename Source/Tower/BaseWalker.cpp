// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseWalker.h"
#include "Components/CapsuleComponent.h"
#include "Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <string>


// Sets default values
ABaseWalker::ABaseWalker(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("Health"));
	Inventory = CreateDefaultSubobject<UInventoryManager>(TEXT("Inventory"));
	HurtBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HurtBox"));

	DamageManager->OnTakeAnyDamage.AddDynamic(this,&ABaseWalker::OnTakeDamage);
	DamageManager->OnDeath.AddDynamic(this,&ABaseWalker::OnDeath);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	TDSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TDSCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	
	HurtBox->InitCapsuleSize(30.0f, 60.0f);
	HurtBox->SetMassOverrideInKg(NAME_None,0.5f,true);
	HurtBox->SetupAttachment(RootComponent);
	HurtBox->SetCollisionProfileName("PawnHurtBox");

	GetCapsuleComponent()->InitCapsuleSize(60.0f, 60.0f);
	
}

void ABaseWalker::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	MyCharacterMovementComponent = Cast<UMyCharacterMovementComponent>(Super::GetMovementComponent());
}

// Called when the game starts or when spawned
void ABaseWalker::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseWalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,UEnum::GetValueAsString(CurState.GetValue()));
}



void ABaseWalker::DodgeInDirection(FVector2D Direction)
{
	MyCharacterMovementComponent->DoDodge(Direction);
}

void ABaseWalker::Attack(FVector Direction)
{
	if (GetMyMovementComponent()->bIsActionable) {
		FVector SpawnPoint = GetActorLocation() + Direction * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f);
		FRotator SpawnRotation = Direction.Rotation();
		FActorSpawnParameters ActorSpawnParams;

		FDamageStats Stats;
		Stats.Instigator = DamageManager;
		Stats.Damage = 10.0f;
		Stats.Mod = 0.25f;
		Stats.DefType = UDamageTypeBase::StaticClass();


		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), SpawnPoint, SpawnRotation, ActorSpawnParams);
		if (IsValid(Bullet))
		{
			Bullet->InitialiseStats(Stats, this);
		}
	}
}

void ABaseWalker::TakeDamage_Implementation(FDamageStats Damage)
{
	DamageManager->TakeDamage(Damage);
}

void ABaseWalker::OnTakeDamage(FDamageResult Result)
{
	LaunchDir = Result.Direction;
	LaunchVelocity += Result.LaunchSpeed;
	for(int i = 0; i<Result.DamageResTags.Num(); i++)
	{
		EDamageResultTags ResultTag = Result.DamageResTags[i];
		switch(ResultTag)
		{
			case EDamageResultTags::DamResTag_Kill:
				GetMyMovementComponent()->Launch(LaunchDir * LaunchVelocity);
			break;
			case EDamageResultTags::DamResTag_Launch:
				GetMyMovementComponent()->Launch(LaunchDir * LaunchVelocity);
			break;
			case EDamageResultTags::DamResTag_Stun:
				GetMyMovementComponent()->Stun(1.0f);
			break;
		}
	}
}

void ABaseWalker::OnDeath(FDamageResult Result)
{
	GetMyMovementComponent()->bIsDead = true;
	GetMyMovementComponent()->Launch(LaunchDir * LaunchVelocity);
}
