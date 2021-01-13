// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "Damage.h"
#include "DamageTypeBase.generated.h"

/**
 * Includes Utility for Damage as well as an overridable function for calculating damage
 */
UCLASS()
class TOWER_API UDamageTypeBase : public UDamageType
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	FColor Colour = FColor::Black;

	UPROPERTY(BlueprintReadOnly)
	float ArmorPenetration;

	FDamageResult CalculateDamage(FDamageStats Damage);

	FDamageResult CalculateArmorPen(float Damage, float Health, float Armor, float ArmorPen);
};

