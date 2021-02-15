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
	FVector LookDirectionFromWorldPoint(FVector WorldPoint);

	void MoveX(float mag);
	void MoveY(float mag);

	void AimX(float mag);
	void AimY(float mag);

	void StartAim();
	void EndAim();

	void Aim();

	/*
	 *Uses Input to determine the point where a controlled walker should look
	 *@return: Either a Normalised Vector or a point in the world, and a bool saying whether it is the former.
	 */
	TPair<FVector,bool> DetermineLookVector();

	void StartAttack();
	void EndAttack();

	void DoAttack();

	void DodgeInput();

	void QuickRestart();

	
	bool bIsAttacking = false;

	FVector MouseLocation;
	FVector MouseDirection;

	FVector2D MoveDir;
	FVector2D AimDir;

	FVector FocusPoint;

	bool bHasShot;
	bool bIsAiming;

	void SetupInputComponent() override;

	ABaseWalker* Walker;

};