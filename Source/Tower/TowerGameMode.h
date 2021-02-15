// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/Widget.h"
#include "GameFramework/GameMode.h"
#include "TowerGameMode.generated.h"

UENUM(BlueprintType)
enum Tower_GameState {
	GameState_PreGame,
	GameState_Paused,
	GameState_InGame,
	GameState_PostGame
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
	TArray<TSubclassOf<UWidget>> HUDElements;
	
};
