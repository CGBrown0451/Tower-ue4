// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Damage.generated.h"


class UDamageTypeBase;
class UDamageManager;

UENUM(BlueprintType)
enum EDamageTags {
	DamTag_Stun,
	DamTag_Strong,
	DamTag_Launch
};

UENUM(BlueprintType)
enum EDamageResultTags
{
	DamResTag_Stun,
	DamResTag_Launch,
	DamResTag_Break,
	DamResTag_Kill,
	DamResTag_Weak,
	DamResTag_Strong
};

USTRUCT(BlueprintType)
struct FDamageResult
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
		UDamageManager* Instigator;
	UPROPERTY(BlueprintReadWrite)
		UDamageManager* Taker;
	UPROPERTY(BlueprintReadWrite)
		float Health;
	UPROPERTY(BlueprintReadWrite)
		float Armor;
	UPROPERTY(BlueprintReadWrite)
		float Balance;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<UDamageTypeBase> Type;
	UPROPERTY(BlueprintReadWrite)
		FVector Position;
	UPROPERTY(BlueprintReadWrite)
		FVector Direction;
	UPROPERTY(BlueprintReadWrite)
		float LaunchSpeed;
	UPROPERTY(BlueprintReadWrite)
		TArray<TEnumAsByte<EDamageResultTags>> DamageResTags;
	
};

USTRUCT(BlueprintType)
struct FDamageStats
{
	GENERATED_BODY()
	UDamageManager* Instigator;
	UDamageManager* Taker;
	float Damage;
	float Mod;
	FVector Direction;
	FVector Position;
	TSubclassOf<UDamageTypeBase> DamageType;
	TArray<EDamageTags> DamageTags;

};


/**
 * 
 */
UCLASS()
class TOWER_API UDamage : public UObject
{
	GENERATED_BODY()
	
};
