// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DamageTypeBase.h"
#include "DamageManager.generated.h"



/*
 * Handles all incoming damage and health calculations. 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOWER_API UDamageManager : public UActorComponent
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnDealAnyDamage, FDamageResult, Result);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnTakeAnyDamage, FDamageResult, Result);

public:	
	// Sets default values for this component's properties
	UDamageManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	float MaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float Health = MaxHealth;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float MaxArmor = 100.0f;
	UPROPERTY(VisibleAnywhere)
	float Armor = 0.0f;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float MaxBalance = 30.0f;
	UPROPERTY(VisibleAnywhere)
	float Balance = MaxBalance;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	TMap<TSubclassOf<UDamageTypeBase>, float> Resistances;
	UPROPERTY(BlueprintReadOnly)
	bool bIsArmored = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsBalanced = true;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//TODO: Stat Effect Class Array

	void TakeDamage(FDamageStats DamageStats);

	void DealDamage(FDamageStats &DamageStats);

	UPROPERTY(BlueprintAssignable)
	FOnDealAnyDamage OnDealAnyDamage;

	UPROPERTY(BlueprintAssignable)
	FOnTakeAnyDamage OnTakeAnyDamage;
	

};
