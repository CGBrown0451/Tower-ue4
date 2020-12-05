// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Math/Vector2D.h"
#include "DamageManager.h"
#include "InventoryManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "BaseWalker.generated.h"

/*
 * The Possible states a Walker can be in.
 */
UENUM()
enum FWalkerState {
	WalkerState_Normal,
	WalkerState_Aiming,
	WalkerState_Dodging,
	WalkerState_HitStun,
	WalkerState_Launched,
	WalkerState_Interacting,
	WalkerState_Melee,
	WalkerState_Dead
};
/*
 * The Base Class for all characters in the game. Includes basic movement code. Recieves input from an ABasePlayerController.
 */
UCLASS()
class TOWER_API ABaseWalker : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseWalker();

	bool actionable;

	FVector2D MoveDir;
	FVector2D AimDir;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<FWalkerState> CurState = WalkerState_Normal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UDamageManager* DamageManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UInventoryManager* Inventory;

	UPROPERTY()
		USpringArmComponent* SpringArm;

	UPROPERTY()
		UCameraComponent* TDSCamera;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DodgeInDirection(FVector2D Direction);

	void WalkInDirection(FVector2D Direction, float DeltaTime);

	void LookInDirection(FVector2D Direction, float DeltaTime, bool Lerp);
	

};
