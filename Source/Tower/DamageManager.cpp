// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageManager.h"
#include <string>

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
	//Check for valid Damage Type class
	if(!DamageStats.DamageType)
	{
		if(DamageStats.DefType)
		{
			DamageStats.DamageType = Cast<UDamageTypeBase>(DamageStats.DefType.GetDefaultObject());
		}else
		{
			//No usable DamageType Class exists, exiting
			return;
		}
	}
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::SanitizeFloat(DamageStats.Damage));
	FDamageResult Result;
	float DamageMult = 1.0f;
	UDamageTypeBase* Type = DamageStats.DamageType;

	//TODO: DealDamage Implementation
	Result.Health = DamageStats.Damage;
	
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::SanitizeFloat(Result.Health));
	if (Type)
	{
		Result = Type->CalculateDamage(DamageStats);
	}
	Result.Taker = this;
	Result.Type = DamageStats.DamageType;
	Result.Position = DamageStats.Position;
	Result.Direction = DamageStats.Direction;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow,Result.Direction.ToString());
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, FString::SanitizeFloat(Result.Health));
	//TODO: Damage Boosters and Effects
	
	if(Health - Result.Health <= 0.0f && bIsAlive)
	{
		Result.DamageResTags.Add(DamResTag_Kill);
		bIsAlive = false;
		OnDeath.Broadcast(Result);
		Health = 0.0f;
	}else
	{
		Health -= Result.Health;
	}

	if (Armor - Result.Armor <= 0.0f && bIsArmored)
	{
		Result.DamageResTags.Add(DamResTag_Break);
		bIsArmored = false;
		OnArmorBroken.Broadcast(Result);
		Armor = 0.0f;
	}else
	{
		Armor -= Result.Armor;
	}

	Armor = FMath::Clamp(Armor, 0.0f, 100.0f);
	
	if (Balance - Result.Balance <= 0.0f && bIsBalanced)
	{
		Result.DamageResTags.Add(DamResTag_Launch);
		bIsBalanced = false;
		OnLostBalance.Broadcast(Result);
		Balance = 0.0f;
	}else
	{
		Balance -= Result.Balance;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::SanitizeFloat(Result.Health));
	
	OnTakeAnyDamage.Broadcast(Result);
	/*
	if(DamageStats.Instigator)
	{
		DamageStats.Instigator->OnDealAnyDamage.Broadcast(Result);
	}
	*/
}

void UDamageManager::DealDamage(FDamageStats &DamageStats)
{
}

void UDamageManager::InitializeComponent()
{
	if (bInitialiseFull)
	{
		Health = MaxHealth;
		Armor = MaxArmor;
		Balance = MaxBalance;
	}
}

