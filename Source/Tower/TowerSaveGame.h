// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Campaign.h"
#include "DamageManager.h"
#include "InventoryManager.h"
#include "GameFramework/SaveGame.h"
#include "TowerSaveGame.generated.h"


USTRUCT(BlueprintType)
struct FCampaignSave
{
	GENERATED_BODY()
	
	UCampaign* Campaign;
	TArray<float> BestTimes;
	int CurrentLevel;
	
	
};

/**
 * Base SaveGame class for Tower
 */
UCLASS()
class TOWER_API UTowerSaveGame : public USaveGame
{
	GENERATED_BODY()

	TArray<FCampaignSave> CampaignSaves;
	
};
