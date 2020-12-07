// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BaseWalker.h"
#include "BasePlayerController.generated.h"


/**
 * The Player Controller for all Walkers. Sends input.
 */
UCLASS()
class TOWER_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	ABasePlayerController();
	
	void OnPossess(APawn* InPawn) override;
	void Tick(float DeltaSeconds) override;

	UFUNCTION()
	FVector2D LookDirectionFromWorldPoint(FVector WorldPoint);

	void MoveX(float mag);
	void MoveY(float mag);

	void AimX(float mag);
	void AimY(float mag);

	void StartAim();
	void EndAim();

	void StartAttack();
	void EndAttack();

	void DodgeInput();

public:
	UPROPERTY()
	bool bIsAttacking;
	bool bIsAiming;

	FVector MouseLocation;
	FVector MouseDirection;

	FVector2D MoveDir;
	FVector2D AimDir;

	FVector FocusPoint;
	
	void SetupInputComponent() override;
	
	
	UPROPERTY()
	ABaseWalker* Walker;
};