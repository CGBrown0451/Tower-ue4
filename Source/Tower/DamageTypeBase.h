// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/DamageType.h"
#include "DamageTypeBase.generated.h"

USTRUCT(BlueprintType)
struct FDamageResult
{
	GENERATED_BODY()
	float Health;
	float Armor;
	float Balance;
};

UENUM()
enum EDamageTags {
	DamTag_Stun,
	DamTag_Strong,
	DamTag_Launch
};

/**
 * Includes Utility for Damage as well as an overridable function for calculating damage
 */
UCLASS()
class TOWER_API UDamageTypeBase : public UDamageType
{
	GENERATED_BODY()
	
	FDamageResult CalculateDamage(float Health, float ArmorPen, FVector Direction);
	
};

