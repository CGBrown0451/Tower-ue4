// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PainCausingVolume.h"
#include "MyPainCausingVolume.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API AMyPainCausingVolume : public APainCausingVolume
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=PainCausingVolume)
	float Mod;

private:
	virtual void CausePainTo(AActor* Other) override;
	
};
