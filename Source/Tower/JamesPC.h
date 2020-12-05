// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JamesPC.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API AJamesPC : public APlayerController
{
	GENERATED_BODY()

	void SetupInputComponent() override;

	void MoveX(float mag);
	void MoveY(float mag);
	
};
