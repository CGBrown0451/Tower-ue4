// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterMovementComponent.h"

#include <Codecapi.h>



#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"


UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
	bUseControllerDesiredRotation = true;
	
	WallSlideQueryParams.bIgnoreTouches = true;

	bCanWalkOffLedges = false;
	PerchRadiusThreshold = 60.0f;
	Mass = 0.5f;
	AirControl = 0.0f;
}

void UMyCharacterMovementComponent::BeginPlay()
{
	DodgeCurveFloat = DodgeCurve.LoadSynchronous();	
}

void UMyCharacterMovementComponent::PhysWalking(float deltaTime, int32 Iterations)
{
	Super::PhysWalking(deltaTime, Iterations);
	LerpToDirection(GetLastInputVector(),deltaTime);
	TransitionToDeath();
	
}

void UMyCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
	
	switch(CustomMovementMode){
		case CMOVE_Dodge:
			PhysDodge(deltaTime, Iterations);
		break;
		case CMOVE_WallSlide:
			PhysSlide(deltaTime,Iterations);
		break;
		case CMOVE_Aiming:
			PhysAiming(deltaTime, Iterations);
		break;
		case CMOVE_Stun:
			PhysStun(deltaTime, Iterations);
		break;
		case CMOVE_Launched:
			PhysLaunched(deltaTime, Iterations);
		break;
		case CMOVE_Dead:
			PhysDead(deltaTime, Iterations);
		break;
	}
	
}

bool UMyCharacterMovementComponent::DoDodge(FVector2D Direction)
{
	if (bIsActionable && !bIsCurrentlyDodging)
	{
		SetMovementMode(MOVE_Custom, CMOVE_Dodge);
		if(CustomMovementMode == CMOVE_WallSlide)
		{
			//TODO: Calculate the new angle of a wallslide dodge
		} else
		{
			if (Direction.Size() > 0.1f)
			{
				Direction.Normalize();
				DodgeDirection = Direction;
				GetCharacterOwner()->GetController()->SetControlRotation(FVector(Direction,0.0f).Rotation());
			}else
			{
				FVector BackDir = GetLastUpdateRotation().Vector() * -1.0f;
				DodgeDirection = FVector2D(BackDir.X,BackDir.Y);
				GetCharacterOwner()->GetController()->SetControlRotation(BackDir.Rotation());
			}
		}
		
		return true;
	}
	return false;
}

const bool UMyCharacterMovementComponent::CanDodge()
{
	return bIsActionable && !bIsCurrentlyDodging;
}

void UMyCharacterMovementComponent::Stun(float Time)
{
	FFindFloorResult Out;
	FindFloor(LastUpdateLocation,Out,true);
	if(!Out.IsWalkableFloor())
	{
		Velocity = FVector::ZeroVector;
		SetMovementMode(MOVE_Falling);
		return;
	}
	SetMovementMode(MOVE_Custom,CMOVE_Stun);
	StunTimeLeft = Time;
	StunSet = false;
}

void UMyCharacterMovementComponent::Launch(FVector const& LaunchVel)
{
	LaunchVelocity = LaunchVel;
	SetMovementMode(MOVE_Custom,CMOVE_Launched);
}

void UMyCharacterMovementComponent::LerpToDirection(FVector Direction, float deltaTime)
{
	if(Direction.Size() > 0.1f)
	{
		FRotator Rotator = FMath::RInterpTo(GetCharacterOwner()->GetController()->GetControlRotation(),Direction.Rotation(),deltaTime,10.0f * Direction.Size());
		GetCharacterOwner()->GetController()->SetControlRotation(Rotator);
	}
}

FRotator UMyCharacterMovementComponent::ReactToDirection(FVector Direction)
{
	Direction.Normalize();
	FVector RecievedDirection = GetCharacterOwner()->GetController()->GetControlRotation().Vector();
	float foredir = FVector::DotProduct(Direction, Direction * -1);
	float aftdir = FVector::DotProduct(Direction * -1, Direction * -1);

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::SanitizeFloat(foredir), true);
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::White, FString::SanitizeFloat(aftdir), true);
	if (foredir < aftdir)
	{
		return RecievedDirection.Rotation();
	}else
	{
		RecievedDirection *= -1;
		return RecievedDirection.Rotation();
	}
}


void UMyCharacterMovementComponent::LookAtPoint(FVector Point, float Time)
{
	LookPoint = Point;
	LookTime = Time;
	bAimStarted = true;
	if(bIsActionable && IsMovingOnGround())
	{
		FVector ActorPoint = GetCharacterOwner()->GetActorLocation();

		FVector Direction = LookPoint - ActorPoint;
		Direction.Normalize();
	
		GetCharacterOwner()->GetController()->SetControlRotation(Direction.Rotation());

		bIsDirection = false;
		
		SetMovementMode(MOVE_Custom,CMOVE_Aiming);
	}
}

void UMyCharacterMovementComponent::LookInDirection(FVector Vector, float Time)
{
	LookPoint = Vector;
	LookTime = Time;
	bAimStarted = true;

	if(bIsActionable && IsMovingOnGround())
	{
		Vector.Normalize();
		GetCharacterOwner()->GetController()->SetControlRotation(Vector.Rotation());
		
		bIsDirection = true;

		SetMovementMode(MOVE_Custom,CMOVE_Aiming);
	}
	
}

void UMyCharacterMovementComponent::PhysDodge(float deltaTime, int32 Iterations)
{
	DodgeTime += deltaTime;
	float DesiredVelocity;
	if(DodgeCurveFloat){
	    DesiredVelocity = DodgeCurveFloat->GetFloatValue(DodgeTime) * DodgeMaxSpeed;
	}else{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Green,"None");
	    DesiredVelocity = DodgeMaxSpeed;
	}
	FVector MovementVector = GetLastInputVector();

	FVector Direction = FVector(DodgeDirection,0.0f);
	Direction = FMath::Lerp(Direction,Direction + MovementVector,DodgeMaxInfluence * deltaTime);
	if(Direction.Size() > 1.0f)
	{
		Direction.Normalize();
	}

	//CalcVelocity(deltaTime,GroundFriction * 0.8f,false,GetMaxBrakingDeceleration());

	FVector Delta = DesiredVelocity * Direction;

	if(Delta.Size() > DodgeMaxSpeed)
	{
		Delta.Normalize();
		Delta *= DodgeMaxSpeed;
	}
	
	FHitResult Out;
	SafeMoveUpdatedComponent(Delta * deltaTime,LastUpdateRotation,true,Out,ETeleportType::None);

	if(Out.bBlockingHit)
	{
		FVector DeltaNormal = Delta;
		DeltaNormal.Normalize();
		float dot = FVector::DotProduct(DeltaNormal,Out.ImpactNormal);
		if(dot < -0.8)
		{
			DeltaNormal = DeltaNormal - 2 * dot * Out.ImpactNormal;
			GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::SanitizeFloat(Delta.Size()));
			Launch(DeltaNormal * Delta.Size());
			return;
		}
		//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::SanitizeFloat(dot));
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Green,"Hit!!");
		FVector Desired =  DeltaNormal - (FVector::DotProduct(DeltaNormal,Out.ImpactNormal)) * Out.ImpactNormal;
		WallSlideVelocity = Desired * Delta.Size();
		GetCharacterOwner()->GetController()->SetControlRotation(ReactToDirection(Desired));
		WallDirection = Out.Normal * -1;
		SetMovementMode(MOVE_Custom, CMOVE_WallSlide);
		return;
	}

	if(DodgeTime > DodgeMinTransitionTime)
	{
		bIsActionable = true;
	}
	
	if(DodgeTime >= DodgeMaxTime)
	{
		SetMovementMode(MOVE_Falling);
		Velocity = Delta; 
		bIsCurrentlyDodging = false;
		DodgeTime = 0.0f;
	}
	
}

void UMyCharacterMovementComponent::PhysSlide(float deltaTime, int32 Iterations)
{
	//Slow down according to wallslide friction
	WallSlideVelocity *= 1 - (WallSlideFriction * deltaTime);
	FVector Delta = WallSlideVelocity;

	//Cap speed
	float speed = Delta.Size();
	if(speed > DodgeMaxSpeed)
	{
		Delta.Normalize();
		Delta *= DodgeMaxSpeed;
	}
	
	//Call component's movement, return a Hit Event
	FHitResult Out;
	LerpToDirection(Delta, deltaTime);
	SafeMoveUpdatedComponent(Delta * deltaTime,LastUpdateRotation,true,Out,ETeleportType::None);
	//If we hit a wall, bonk!!!
	if(Out.bBlockingHit)
	{
		FVector DeltaNormal = Delta;
		DeltaNormal.Normalize();
		float dot = FVector::DotProduct(DeltaNormal,Out.ImpactNormal);
		if(dot < -0.8)
		{
			DeltaNormal = DeltaNormal - 2 * dot * Out.ImpactNormal;
			Launch(DeltaNormal * speed);
			return;
		}
	}
	//Check we are still on a wall
	float radius = GetCharacterOwner()->GetCapsuleComponent()->GetScaledCapsuleRadius() * 1.1f;
	FVector RayEnd =  GetActorLocation() + WallDirection * radius;
	FHitResult WallCheck;
	if(GetWorld()->LineTraceSingleByChannel(WallCheck,GetActorLocation(),RayEnd,ECollisionChannel::ECC_Visibility,WallSlideQueryParams))
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,FString::SanitizeFloat(WallCheck.Time));
	}else
	{
		//Cancel WallSlide if we run out of wall
		SetMovementMode(MOVE_Falling);
		Velocity = Delta;
	}
	//Cancel WallSlide if we're below a set speed 
	if (speed < WallSlideDropoffSpeed)
	{
		SetMovementMode(MOVE_Falling);
		Velocity = Delta;
	}
	
}

void UMyCharacterMovementComponent::PhysAiming(float deltaTime, int32 Iterations)
{
	//Use Super Walking movement
	Super::PhysWalking(deltaTime,Iterations);
	//Count time down if this is a timed aim
	if(!bAimStarted)
	{
		if(LookTime < 0.0f)
		{
			SetMovementMode(MOVE_Walking);
		}
		LookTime -= deltaTime;
	}

	bAimStarted = false;
	//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::SanitizeFloat(LookTime));
	
	//Determine whether we're looking at a point or in a certain directon
	if(bIsDirection)
	{
		//Look in a direction
		GetCharacterOwner()->GetController()->SetControlRotation(LookPoint.Rotation());
	}else
	{
		//Convert point to direction
		FVector ActorPoint = GetCharacterOwner()->GetActorLocation();
		FVector Direction = LookPoint - ActorPoint;
		Direction.Normalize();
		//Look in direction
		GetCharacterOwner()->GetController()->SetControlRotation(Direction.Rotation());
	}

	TransitionToDeath();
	
}

void UMyCharacterMovementComponent::PhysStun(float deltaTime, int32 Iterations)
{
	//Count down stun
	if(!StunSet)
	{
		if(StunTimeLeft < 0.0f)
		{
			SetMovementMode(MOVE_Falling);
		}
	}
	
	StunSet = false;
	StunTimeLeft -= deltaTime;
	
	//Move using the anim root
	FHitResult Out;
	SafeMoveUpdatedComponent(AnimRootMotionVelocity * deltaTime,LastUpdateRotation,true,Out);
}

void UMyCharacterMovementComponent::PhysImpaired(float deltaTime, int32 Iterations)
{
	
}

void UMyCharacterMovementComponent::PhysLaunched(float deltaTime, int32 Iterations)
{
	LaunchVelocity *= 1 - (LaunchedFriction * deltaTime);
	GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Yellow,FString::SanitizeFloat(LaunchVelocity.Size()));
	FHitResult Out;
	LerpToDirection(ReactToDirection(LaunchVelocity).Vector(),deltaTime);
	SafeMoveUpdatedComponent(LaunchVelocity*deltaTime,ReactToDirection(LaunchVelocity),true,Out);
	if(Out.bBlockingHit)
	{
		FVector VelocityNormal = LaunchVelocity;
		VelocityNormal.Normalize();
		VelocityNormal = VelocityNormal - 2 * FVector::DotProduct(VelocityNormal,Out.ImpactNormal) * Out.ImpactNormal;
		VelocityNormal.Normalize();
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,FString::SanitizeFloat(192.99f));
		LaunchVelocity = VelocityNormal * LaunchVelocity.Size() * LaunchedBumpVelocityMultiplier;
	}

	if(LaunchVelocity.Size() < FlooredSpeed)
	{
		FFindFloorResult FloorResult;
		FindFloor(LastUpdateLocation,FloorResult,true);
		if(!FloorResult.IsWalkableFloor())
		{
			Velocity = LaunchVelocity;
			SetMovementMode(MOVE_Falling);
			return;
		}
		Velocity = FVector::ZeroVector;
		Stun(FlooredSpeed * .01f);
	}
	
}

void UMyCharacterMovementComponent::PhysDead(float deltaTime, int32 Iterations)
{
	FHitResult Out;
	SafeMoveUpdatedComponent(AnimRootMotionVelocity * deltaTime,LastUpdateRotation,true,Out);
}

void UMyCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	//Previous Mode
	if (PreviousMovementMode == EMovementMode::MOVE_Custom)
	{
		switch(PreviousCustomMode)
		{
			case CMOVE_Dodge:
				DodgeTime = 0.0f;
				bIsCurrentlyDodging = false;
			break;
		}
	}

	//Current Mode
	if (MovementMode == MOVE_Custom)
	{
		//Custom Modes
		switch(CustomMovementMode)
		{
			case CMOVE_Dodge:
				bIsActionable = false;
				bIsCurrentlyDodging = true;
			break;
			case CMOVE_WallSlide:
				bIsActionable = true;
				bIsCurrentlyDodging = false;
			break;
			case CMOVE_Launched:
				Velocity = FVector::ZeroVector;
			break;
			case CMOVE_Dead:
				bIsActionable = false;
			break;
		}
	}else
	{
		//Official Modes
		switch(MovementMode)
		{
			case MOVE_Walking:
				TransitionToDeath();
				bIsActionable = true;
			break;
			case MOVE_Falling:
				bIsActionable = false;
			break;
		}
	}
	
}

void UMyCharacterMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	/*
	if(MovementMode == MOVE_Custom)
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,FString::FromInt(CustomMovementMode));
	}else
	{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,UEnum::GetValueAsString(MovementMode.GetValue()));
	}
	*/
}

void UMyCharacterMovementComponent::TransitionToDeath()
{
	if(bIsDead)
	{
		SetMovementMode(MOVE_Custom,CMOVE_Dead);
	}
}
