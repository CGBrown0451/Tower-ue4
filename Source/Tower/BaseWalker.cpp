// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseWalker.h"
#include "Components/CapsuleComponent.h"
#include "Bullet.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <string>


// Sets default values
ABaseWalker::ABaseWalker()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageManager = CreateDefaultSubobject<UDamageManager>(TEXT("Health"));
	Inventory = CreateDefaultSubobject<UInventoryManager>(TEXT("Inventory"));
	HurtBox = CreateDefaultSubobject<UCapsuleComponent>(TEXT("HurtBox"));

	DamageManager->OnTakeAnyDamage.AddDynamic(this,&ABaseWalker::OnTakeDamage);

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
	
	HurtBox->InitCapsuleSize(30.0f, 60.0f);
	HurtBox->SetupAttachment(RootComponent);
	HurtBox->SetCollisionProfileName("PawnHurtBox");

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
	//Walker States
	switch (CurState){

	case WalkerState_Normal:
		WalkInDirection(MoveDir, DeltaTime);
		if (MoveDir.Size() > 0.1f) 
		{
			LookInDirection(MoveDir, DeltaTime, true, 1.0f);
		}
		break;
	case WalkerState_Aiming:
		WalkInDirection(MoveDir, DeltaTime);
		LookInDirection(AimDir, DeltaTime, false, 1.0f);
		break;
	case WalkerState_HitStun:
		DoWobble();
		break;

	}
	//Camera States
	switch(CurCameraState)
	{
	case CameraState_Normal:
		break;
	case CameraState_Looking:
		break;
	}

	HighestPriorityLook = 0.0f;
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

void ABaseWalker::LookInDirection(FVector2D Direction, float DeltaTime, bool Lerp, float Priority)
{
	if (Priority < HighestPriorityLook)
	{
		return;
	}
	HighestPriorityLook = Priority;
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, FString::SanitizeFloat(Priority));
		if (Lerp)
		{
			GetController()->SetControlRotation(FMath::RInterpTo(GetControlRotation(), FVector(Direction, 0.0f).Rotation(), DeltaTime, 10.0f * Direction.Size()));
		}
		else
		{
			GetController()->SetControlRotation(FVector(Direction, 0.0f).Rotation());
		}
}

void ABaseWalker::AttackInDirection(FVector2D Direction)
{
	if (bIsActionable) {
		LookInDirection(Direction, 0.0f, false, 2.0f);
		FVector SpawnPoint = GetActorLocation() + GetControlRotation().Vector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.5f);
		FRotator SpawnRotation = GetControlRotation();
		FActorSpawnParameters ActorSpawnParams;

		FDamageStats Stats;
		Stats.Instigator = DamageManager;
		Stats.Damage = 10.0f;
		Stats.Mod = 0.25f;
		Stats.DamageType = UDamageTypeBase::StaticClass();


		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ABullet* Bullet = GetWorld()->SpawnActor<ABullet>(ABullet::StaticClass(), SpawnPoint, GetControlRotation(), ActorSpawnParams);
		if (IsValid(Bullet))
		{
			Bullet->InitialiseStats(Stats, this);
		}
	}
}

void ABaseWalker::ReactToDirection(FVector2D Direction)
{
	FVector RecievedDirection = GetControlRotation().Vector();
	FVector2D FacingDirection = FVector2D(RecievedDirection.X,RecievedDirection.Y);
	float foredir = FVector2D::DotProduct(FacingDirection, Direction * -1);
	float aftdir = FVector2D::DotProduct(FacingDirection * -1, Direction * -1);

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::SanitizeFloat(foredir), true);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::SanitizeFloat(aftdir), true);
	if (foredir < aftdir)
	{
		LookInDirection(Direction, 0.0f, false, 5.0f);
	}else
	{
		LookInDirection(Direction * -1, 0.0f, false, 5.0f);
	}
}

void ABaseWalker::StunForXSeconds(float duration)
{
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle, this, &ABaseWalker::StopStun, duration);
	SetWalkerState(WalkerState_HitStun);
}

void ABaseWalker::StopStun()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Reset", true);
	UnWobble();
	bIsActionable = true;
	ResetWalkerState();
}

void ABaseWalker::DoWobble()
{
	FVector Wobble = FVector(0.0f, FMath::Cos(GetGameTimeSinceCreation()*20) * 10.0f, 0.0f);
	if (IsValid(WobbleComponent))
	{
		WobbleComponent->SetRelativeLocation(Wobble);
	}
}

void ABaseWalker::UnWobble()
{
	if (IsValid(WobbleComponent))
	{
		WobbleComponent->SetRelativeLocation(FVector::ZeroVector);
	}
}

bool ABaseWalker::SetWalkerState(TEnumAsByte<EWalkerState> NewState)
{
	if (NewState == WalkerState_Normal || NewState == WalkerState_Aiming || NewState == WalkerState_NormalNav)
	{
		if (bIsActionable)
		{
			CurState = NewState;
			return true;
		}else
		{
			return false;
		}
	}else
	{
		bIsActionable = false;
		CurState = NewState;
		return true;
	}
}

bool ABaseWalker::SetCameraState(TEnumAsByte<ECameraState> NewState)
{
	CurCameraState = NewState;
	return true;
}

TEnumAsByte<EWalkerState> ABaseWalker::ResetWalkerState()
{
	
	if (DamageManager->Balance <= 0.0f)
	{
		return WalkerState_Launched;
	}
	
	if(bIsActionable)
	{
		if (DamageManager->Health <= 0.0f)
		{
			return WalkerState_Dead;
		}
		
		if (SetWalkerState(WalkerState_Normal))
		{
			return WalkerState_Normal;
		}
		
	}
	return WalkerState_Normal;
}

void ABaseWalker::TakeDamage_Implementation(FDamageStats Damage)
{
	DamageManager->TakeDamage(Damage);
}

void ABaseWalker::OnTakeDamage(FDamageResult Result)
{
	StunForXSeconds(.5f);
	ReactToDirection(FVector2D(Result.Direction.X,Result.Direction.Y));
}