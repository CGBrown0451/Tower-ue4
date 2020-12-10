// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"

// Sets default values for this component's properties
UDamageManager::UDamageManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDamageManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// ...
}

void UDamageManager::TakeDamage(FDamageStats DamageStats)
{
	FDamageResult Result;
	float DamageMult = 1.0f;
	float* DamageMultPtr = Resistances.Find(DamageStats.DamageType);
	if(DamageMultPtr)
	{
		DamageMult = *DamageMultPtr;
	}
	UDamageTypeBase* Type = DamageStats.DamageType.GetDefaultObject();

	if (IsValid(DamageStats.Instigator))
	{
		DamageStats.Instigator->DealDamage(DamageStats);
	}else
	{
		Result.Health = DamageStats.Damage;
	}
	
	if (IsValid(Type))
	{
		Result = Type->CalculateDamage(DamageStats);
	}
	Result.Taker = this;
	Result.Type = DamageStats.DamageType;
	Result.Position = DamageStats.Position;

	//TODO: Damage Boosters and Effects
	
	Health -= Result.Health;
	if(Health <= 0.0f)
	{
		Result.DamageResTags.Add(DamResTag_Kill);
		bIsAlive = false;
	}

	Armor -= Result.Armor;
	if (Armor <= 0.0f)
	{
		Result.DamageResTags.Add(DamResTag_Break);
		bIsArmored = false;
	}

	Balance -= Result.Balance;
	if (Balance <= 0.0f)
	{
		Result.DamageResTags.Add(DamResTag_Launch);
		bIsBalanced = false;
	}
	
	OnTakeAnyDamage.Broadcast(Result);
	if(IsValid(DamageStats.Instigator))
	{
		DamageStats.Instigator->OnDealAnyDamage.Broadcast(Result);
	}
}

void UDamageManager::DealDamage(FDamageStats &DamageStats)
{
}

