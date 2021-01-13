// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Campaign.generated.h"

USTRUCT(BlueprintType)
struct FLevelProperties
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText LevelSafeName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TAssetPtr<UWorld> Level;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* LevelPicture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bLeadsOntoNext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BronzeScore;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SilverScore;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GoldScore;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AuthorScore;
	
};

UENUM(BlueprintType)
enum EScoreResultType{
	ScoreResultType_None,
	ScoreResultType_Bronze,
	ScoreResultType_Silver,
	ScoreResultType_Gold,
	ScoreResultType_Author
};


/**
 * A Data only class, meant to store data on levelsets or campaigns. Is used in the GameInstance to determine flow between levels. Best used in Blueprint.
 */
UCLASS(Blueprintable, BlueprintType)
class TOWER_API UCampaign : public UObject
{
	GENERATED_BODY()
	UCampaign();
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText CampaignSafeName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsLevelSet;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAreHigherScoresBetter;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bFormatScoreAsTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FLevelProperties> Levels;

	UFUNCTION(BlueprintCallable)
	EScoreResultType CheckScore(float Score, FLevelProperties Level);

	UFUNCTION(BlueprintCallable)
	FLevelProperties GetLevelAtPosition(int Position);

	
	
};
