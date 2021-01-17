// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacterMovementComponent.h"


UMyCharacterMovementComponent::UMyCharacterMovementComponent()
{
}

void UMyCharacterMovementComponent::BeginPlay()
{
	DodgeCurveFloat = DodgeCurve.LoadSynchronous();	
}

void UMyCharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
	
	switch(CustomMovementMode){
		case 0:
			DodgeUpdate(deltaTime, Iterations);
		break;
	}
	
}

bool UMyCharacterMovementComponent::DoDodge(FVector2D Direction)
{
	if (IsMovingOnGround() &&  bIsActionable && !bIsCurrentlyDodging)
	{
		SetMovementMode(MOVE_Custom, CMODE_Dodge);
		if (Direction.Size() > 0.1f)
		{
			Direction.Normalize();
			DodgeDirection = Direction;
		}else
		{
			FVector BackDir = GetLastUpdateRotation().Vector() * -1.0f;
			DodgeDirection = FVector2D(BackDir.X,BackDir.Y);
		}
		return true;
	}
	return false;
}

void UMyCharacterMovementComponent::DodgeUpdate(float deltaTime, int32 Iterations)
{
	DodgeTime += deltaTime;
	float DesiredVelocity;
	if(DodgeCurveFloat){
	    DesiredVelocity = DodgeCurveFloat->GetFloatValue(DodgeTime) * DodgeMaxSpeed * deltaTime;
	}else{
		GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Green,"None");
	    DesiredVelocity = DodgeMaxSpeed * deltaTime;
	}
	FVector MovementVector = GetLastInputVector();

	FVector Direction = FVector(DodgeDirection,0.0f);
	Direction = FMath::Lerp(Direction,Direction + MovementVector,MaxInfluence * deltaTime);
	if(Direction.Size() > 1.0f)
	{
		Direction.Normalize();
	}

	CalcVelocity(deltaTime,GroundFriction * 0.8f,false,GetMaxBrakingDeceleration());
	
	FHitResult Out;
	SafeMoveUpdatedComponent(Direction * DesiredVelocity,LastUpdateRotation,true,Out,ETeleportType::None);

	if(Out.bBlockingHit)
	{
		
	}

	if(DodgeTime >  DodgeMinTransitionTime)
	{
		bIsActionable = true;
	}
	
	if(DodgeTime >= DodgeMaxTime)
	{
		SetMovementMode(MOVE_Falling);
		DodgeTime = 0.0f;
	}
	
}

void UMyCharacterMovementComponent::OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PreviousMovementMode, PreviousCustomMode);
	//Previous Mode
	if (PreviousMovementMode == EMovementMode::MOVE_Custom)
	{
		switch(PreviousCustomMode)
		{
			case 0:
				DodgeTime = 0.0f;
			break;
		}
	}

	//Current Mode
	if (MovementMode == MOVE_Custom)
	{
		switch(CustomMovementMode)
		{
			case CMODE_Dodge:
				bIsActionable = false;
			break;
		}
	}else
	{
		switch(MovementMode)
		{
			case MOVE_Walking:
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
	//GEngine->AddOnScreenDebugMessage(-1,1.0f,FColor::Red,UEnum::GetValueAsString(MovementMode.GetValue()));
}
