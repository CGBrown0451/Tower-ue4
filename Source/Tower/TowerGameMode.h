// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TowerGameMode.generated.h"

UENUM(BlueprintType)
enum Tower_GameState {
	GameState_PreGame,
	GameState_InGame,
	GameState_PostGame
};

UENUM(BlueprintType)
enum Tower_HUDElements{
	HUD_HealthAndArmor,
	HUD_Timer,
	HUD_AmmoCounter,
	HUD_ScoreCounter
};

/**
 * 
 */
UCLASS()
class TOWER_API ATowerGameMode : public AGameMode
{
	GENERATED_BODY()
	ATowerGameMode();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TEnumAsByte<Tower_HUDElements>> HUDElements;
	
};
