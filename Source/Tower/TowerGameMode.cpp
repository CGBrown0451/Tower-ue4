// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerGameMode.h"
#include "DebugBaseWalker.h"
#include "BasePlayerController.h"
#include "TowerGameState.h"

ATowerGameMode::ATowerGameMode()
{
	DefaultPawnClass = ADebugBaseWalker::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
	GameStateClass = ATowerGameState::StaticClass();
}
