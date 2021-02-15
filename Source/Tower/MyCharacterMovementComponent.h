// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"
UENUM(BlueprintType)
enum ECustomMovementMode
{
	CMOVE_Dodge,
	CMOVE_WallSlide,
	CMOVE_Aiming,
	CMOVE_Stun,
	CMOVE_Impaired,
	CMOVE_Launched,
	CMOVE_Floored,
	CMOVE_Dead
};

/**
 * 
 */
UCLASS()
class TOWER_API UMyCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UMyCharacterMovementComponent();

	virtual void BeginPlay() override;

	virtual void PhysWalking(float deltaTime, int32 Iterations) override;
	
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	//Dodge. Follows a CurveFloat for velocity. Transitions into landing when finished. Transitions into WallSlide or Launch upon hitting a wall depending on Contact Normal
	void PhysDodge(float deltaTime, int32 Iterations);
	//WallSlide. Slows down at a consistent speed. Can chain into another dodge. Transitions out upon not sensing a wall or hitting another wall.
	void PhysSlide(float deltaTime, int32 Iterations);
	//Aiming. Not much different to walking other than looking at a certain point.
	void PhysAiming(float deltaTime, int32 Iterations);
	//Stun. Stops input from being processed for a certain amount of time.
	void PhysStun(float deltaTime, int32 Iterations);
	//Impaired. Slower version of regular movement.
	void PhysImpaired(float deltaTime, int32 Iterations);
	//Launched. Moves the character over a plain and bounces them off any contacted surface.
	void PhysLaunched(float deltaTime, int32 Iterations);
	//Dead. The player can't move out of this state.
	void PhysDead(float deltaTime, int32 Iterations);
	
	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	//Checks if bIsDead is true and transitions to the Death State if true
	void TransitionToDeath();
public:
	bool DoDodge(FVector2D Direction);
	
	const bool CanDodge();

	void Stun(float Time);
	//Overridden from UCharacterMovementComponent. Stores passed in vector and changes movement state to launched.
	void Launch(FVector const& LaunchVel) override;

	void LerpToDirection(FVector Direction, float deltaTime);

	FRotator ReactToDirection(FVector Direction);

	void LookAtPoint(FVector Point, float Time);

	void LookInDirection(FVector Vector, float Time);

	bool bIsActionable = true;

	bool bIsDead = false;
	
//Dodge Properties

public:
	UPROPERTY(EditAnywhere,Category="Custom Movement: Dodge",BlueprintReadOnly)
	float DodgeMaxSpeed = 500.0f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Dodge",BlueprintReadOnly)
	float DodgeMaxTime = 0.8f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Dodge",BlueprintReadOnly)
	float DodgeMaxInfluence = 0.5f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Dodge",BlueprintReadOnly)
	float DodgeMinTransitionTime = 0.8f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Dodge",BlueprintReadOnly)
	TAssetPtr<UCurveFloat> DodgeCurve;
	
	UPROPERTY()
	UCurveFloat* DodgeCurveFloat;
private:
	
	bool bIsCurrentlyDodging;
	
	FVector2D DodgeDirection;

	float DodgeTime = 0.0f;
//WallSlide Properties
public:
	UPROPERTY(EditAnywhere,Category="Custom Movement: WallSlide",BlueprintReadOnly)
	float WallSlideFriction = 1.0f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: WallSlide",BlueprintReadOnly)
	float WallSlideDropoffSpeed = 100.0f;
private:
	FVector WallDirection;

	FVector WallSlideVelocity;

	FCollisionQueryParams WallSlideQueryParams;
//Aiming Properties
public:
	
private:
	FVector LookPoint;

	float LookTime;

	bool bIsDirection = false;

	bool bAimStarted = false;
//Stun Properties
public:
	
private:
	float StunTimeLeft = 0.0f;
	
	bool StunSet = false;
//Impaired Properties
public:
	UPROPERTY(EditAnywhere,Category="Custom Movement: Impaired",BlueprintReadOnly)
	float ImpairedMaxSpeed = 100.0f;
private:
//Launched Properties
public:
	UPROPERTY(EditAnywhere,Category="Custom Movement: Launched",BlueprintReadOnly)
	float LaunchedMaxSpeed = 2500.0f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Launched",BlueprintReadOnly)
	float LaunchedFriction = 1.0f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Launched",BlueprintReadOnly)
	float FlooredSpeed = 100.0f;
	UPROPERTY(EditAnywhere,Category="Custom Movement: Launched",BlueprintReadOnly)
	float LaunchedBumpVelocityMultiplier = 0.8;
private:
	bool bWantsToRecover = false;

	FVector LaunchVelocity;
};
