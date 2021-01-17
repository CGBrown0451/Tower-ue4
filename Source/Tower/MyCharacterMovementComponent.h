// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyCharacterMovementComponent.generated.h"

enum ECustomMovementMode
{
	CMODE_Dodge,
	CMODE_Stun,
	CMODE_Impaired,
	CMODE_Launched
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
	
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;

	virtual void OnMovementModeChanged(EMovementMode PreviousMovementMode, uint8 PreviousCustomMode) override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
public:
	bool DoDodge(FVector2D Direction);

	void DodgeUpdate(float deltaTime, int32 Iterations);

	bool bIsActionable = true;
	
//Dodge Properties

public:
	UPROPERTY(EditAnywhere,Category="Dodge",BlueprintReadOnly)
	float DodgeMaxSpeed = 500.0f;
	UPROPERTY(EditAnywhere,Category="Dodge",BlueprintReadOnly)
	float DodgeMaxTime = 0.8f;
	UPROPERTY(EditAnywhere,Category="Dodge",BlueprintReadOnly)
	float MaxInfluence = 0.5f;
	UPROPERTY(EditAnywhere,Category="Dodge",BlueprintReadOnly)
	float DodgeMinTransitionTime = 0.8f;
	UPROPERTY(EditAnywhere,Category="Dodge",BlueprintReadOnly)
	TAssetPtr<UCurveFloat> DodgeCurve;
	
	UPROPERTY()
	UCurveFloat* DodgeCurveFloat;
private:
	
	bool bIsCurrentlyDodging;
	
	FVector2D DodgeDirection;

	float DodgeTime = 0.0f;
	
	
};
