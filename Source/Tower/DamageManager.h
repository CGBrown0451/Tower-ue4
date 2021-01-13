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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnDeath, FDamageResult, Result);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnArmorBroken, FDamageResult, Result);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam( FOnLostBalance, FDamageResult, Result);

public:	
	// Sets default values for this component's properties
	UDamageManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float MaxHealth = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Health = MaxHealth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) 
	float MaxArmor = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Armor = 0.0f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxBalance = 30.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Balance = MaxBalance;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<TSubclassOf<UDamageTypeBase>, float> Resistances;
	UPROPERTY(BlueprintReadOnly)
	bool bIsArmored = false;
	UPROPERTY(BlueprintReadOnly)
	bool bIsAlive = true;
	UPROPERTY(BlueprintReadOnly)
	bool bIsBalanced = true;
	UPROPERTY(EditAnywhere)
	bool bInitialiseFull = true;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//TODO: Stat Effect Class Array

	void TakeDamage(FDamageStats DamageStats);

	void DealDamage(FDamageStats &DamageStats);

	void InitializeComponent() override;

	UPROPERTY(BlueprintAssignable)
	FOnDealAnyDamage OnDealAnyDamage;

	UPROPERTY(BlueprintAssignable)
	FOnTakeAnyDamage OnTakeAnyDamage;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnArmorBroken OnArmorBroken;

	UPROPERTY(BlueprintAssignable)
	FOnLostBalance OnLostBalance;
	

};
