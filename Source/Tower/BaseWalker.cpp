// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseWalker.h"
#include "Components/CapsuleComponent.h"
#include "Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ABaseWalker::ABaseWalker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("Health"));
	Inventory = CreateDefaultSubobject<UInventoryManager>(TEXT("Inventory"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetUsingAbsoluteRotation(true);
	SpringArm->TargetArmLength = 1200.0f;
	SpringArm->SetRelativeRotation(FRotator(-90.0f, 90.0f, 0.0f));
	SpringArm->bDoCollisionTest = false;

	TDSCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TDSCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	GetCharacterMovement()->bCanWalkOffLedges = false;
	GetCharacterMovement()->PerchRadiusThreshold = 60.0f;

	GetCapsuleComponent()->InitCapsuleSize(60.0f, 60.0f);
	
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

	switch (CurState){

	case WalkerState_Normal:
		WalkInDirection(MoveDir, DeltaTime);
		LookInDirection(MoveDir, DeltaTime, true);
		break;
	case WalkerState_Aiming:
		WalkInDirection(MoveDir, DeltaTime);
		LookInDirection(AimDir, DeltaTime, false);
		break;
	case WalkerState_HitStun:
		DoWobble();
		break;

	}

}



void ABaseWalker::DodgeInDirection(FVector2D Direction)
{
	if (bIsActionable)
	{
		SetWalkerState(WalkerState_Dodging);
	}
}

//Moves in a direction based on input
void ABaseWalker::WalkInDirection(FVector2D Direction, float DeltaTime)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, Direction.ToString());
	AddMovementInput(FVector(Direction, 10), 1);
	//FVector(Direction.X, Direction.Y, 0)
}

void ABaseWalker::LookInDirection(FVector2D Direction, float DeltaTime, bool Lerp)
{
	if (Direction.Size() > 0.1f) 
	{
		if (Lerp)
		{
			GetController()->SetControlRotation(FMath::RInterpTo(GetControlRotation(), FVector(Direction, 0.0f).Rotation(), DeltaTime, 10.0f));
		}
		else
		{
			GetController()->SetControlRotation(FVector(Direction, 0.0f).Rotation());
		}
	}
}

void ABaseWalker::AttackInDirection(FVector2D Direction)
{
	LookInDirection(Direction, 0.0f, false);
	FVector SpawnPoint = GetActorLocation() + GetControlRotation().Vector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	FRotator SpawnRotation = GetControlRotation();
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(),SpawnPoint,GetControlRotation(),ActorSpawnParams);
}

void ABaseWalker::DoWobble()
{
	FVector Wobble = FVector(0.0f, FMath::Cos(GetGameTimeSinceCreation()*20) * 10.0f, 0.0f);
	WobbleComponent->SetRelativeLocation(Wobble);
}

void ABaseWalker::UnWobble()
{
	WobbleComponent->SetRelativeLocation(FVector::ZeroVector);
}

bool ABaseWalker::SetWalkerState(TEnumAsByte<EWalkerState> NewState)
{
	CurState = NewState;
	return true;
}

TEnumAsByte<EWalkerState> ABaseWalker::ResetWalkerState()
{
	if(bIsActionable)
	{
		if (SetWalkerState(WalkerState_Normal))
		{
			return WalkerState_Normal;
		}
		
	}
	return WalkerState_Normal;
}

void ABaseWalker::TakeDamage_Implementation()
{
	
}

