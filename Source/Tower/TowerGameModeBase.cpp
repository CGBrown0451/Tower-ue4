// Copyright Epic Games, Inc. All Rights Reserved.


#include "TowerGameModeBase.h"
#include "BasePlayerController.h"
#include "DebugBaseWalker.h"
#include "JamesChar.h"

ATowerGameModeBase::ATowerGameModeBase()
{
	//DefaultPawnClass = ADebugBaseWalker::StaticClass();
	DefaultPawnClass = ADebugBaseWalker::StaticClass();
	PlayerControllerClass = ABasePlayerController::StaticClass();
}
