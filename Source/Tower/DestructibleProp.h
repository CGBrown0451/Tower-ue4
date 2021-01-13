// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageTaker.h"
#include "GameplayTagContainer.h"
#include "DestructibleProp.generated.h"

UCLASS()
class TOWER_API ADestructibleProp : public AActor, public IDamageTaker
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestructibleProp();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDamageManager* DamageManager;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeDamage_Implementation(FDamageStats Stats) override;

};
