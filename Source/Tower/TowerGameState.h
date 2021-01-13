// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "TowerGameMode.h"
#include "TowerGameState.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API ATowerGameState : public AGameState
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TEnumAsByte<Tower_GameState> GameState;

	UPROPERTY(BlueprintReadOnly)
	float GameTime;

	void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
	void StartGame();
	
	UFUNCTION(BlueprintCallable)
	void EndGame();
	
	void Tick(float DeltaSeconds) override;
	
};
