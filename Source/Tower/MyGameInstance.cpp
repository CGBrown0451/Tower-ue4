// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UMyGameInstance::OnStart()
{
	Super::OnStart();
	//Load stuff, Saves, etc.
}

void UMyGameInstance::Shutdown()
{
}

bool UMyGameInstance::GotoLevelInCampaign(UCampaign* Campaign, int LevelNumber)
{
	CurrentCampaign = Campaign;
	CurrentCampaignLevel = LevelNumber;
	CurrentLevel = Campaign->Levels[LevelNumber];
	UGameplayStatics::OpenLevel(this, FName(Campaign->Levels[LevelNumber].Level.GetAssetName()), false);
	return true;
}

bool UMyGameInstance::GotoNextLevelInCampaign()
{
	if (CurrentCampaign->Levels.Num() > CurrentCampaignLevel + 1)
	{
		UGameplayStatics::OpenLevel(this, FName(CurrentCampaign->Levels[CurrentCampaignLevel + 1].Level.GetAssetName()), false);
		return true;
	}else
	{
		return false;
	}
	
}


