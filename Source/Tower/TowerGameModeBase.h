// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TowerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_API ATowerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ATowerGameModeBase();

	void Tick(float DeltaSeconds) override;

};
