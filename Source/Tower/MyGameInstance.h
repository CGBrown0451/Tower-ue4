// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "TowerSaveGame.h"
#include "Campaign.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

	void OnStart() override;

	void Shutdown() override;
	
public:
	UPROPERTY(BlueprintReadWrite)
	UCampaign* CurrentCampaign;
	UPROPERTY(BlueprintReadWrite)
	int CurrentCampaignLevel;
	UPROPERTY(BlueprintReadWrite)
	FLevelProperties CurrentLevel;

	UFUNCTION(BlueprintCallable)
	bool GotoLevelInCampaign(UCampaign* Campaign, int LevelNumber);

	UFUNCTION(BlueprintCallable)
	bool GotoNextLevelInCampaign();

	
};
