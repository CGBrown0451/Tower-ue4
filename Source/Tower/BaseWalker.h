// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Math/Vector2D.h"
#include "DamageManager.h"
#include "InventoryManager.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DamageTaker.h"
#include "BaseWalker.generated.h"

/*
 * The Possible states a Walker can be in.
 */
UENUM()
enum EWalkerState {
	WalkerState_Normal,
	WalkerState_NormalNav,
	WalkerState_Aiming,
	WalkerState_Dodging,
	WalkerState_HitStun,
	WalkerState_Launched,
	WalkerState_Interacting,
	WalkerState_Melee,
	WalkerState_Dead
};
UENUM()
enum ECameraState{
	CameraState_Normal,
	CameraState_Looking,
	CameraState_Emphasis,
	CameraState_Attracted,
	CameraState_Other
};
/*
 * The Base Class for all characters in the game. Includes basic movement code. Recieves input from an ABasePlayerController.
 */
UCLASS()
class TOWER_API ABaseWalker : public ACharacter, public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseWalker();

	bool bIsActionable = true;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool bIsImmortal = false;

	FVector2D MoveDir;
	FVector2D AimDir;

	FVector2D LaunchDir;
	float LaunchVelocity;

	FVector2D DodgeDirection;
	float DodgeVelocity = 100.0f;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EWalkerState> CurState = WalkerState_Normal;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<ECameraState> CurCameraState = CameraState_Normal;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UDamageManager* DamageManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UInventoryManager* Inventory;

	UPROPERTY()
	USpringArmComponent* SpringArm;

	UPROPERTY()
	UCameraComponent* TDSCamera;

	UPROPERTY()
		UCapsuleComponent* HurtBox;

	UPROPERTY()
	USceneComponent* WobbleComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	float HighestPriorityLook;

	//void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DodgeInDirection(FVector2D Direction);

	void DoDodge(float Deltatime);

	void WalkInDirection(FVector2D Direction, float DeltaTime);

	void LookInDirection(FVector2D Direction, float DeltaTime, bool Lerp, float Priority);

	void AttackInDirection(FVector2D Direction);

	void ReactToDirection(FVector2D Direction);

	void StunForXSeconds(float duration);
	UFUNCTION()
	void StopStun();

	void DoWobble();
	void UnWobble();

	bool SetWalkerState(TEnumAsByte<EWalkerState> NewState);

	bool SetCameraState(TEnumAsByte<ECameraState> NewState);
	UFUNCTION()
	TEnumAsByte<EWalkerState> ResetWalkerState();
	UFUNCTION()
	void TakeDamage_Implementation(FDamageStats Damage) override;
	UFUNCTION()
	void OnTakeDamage(FDamageResult Result);
	

};
