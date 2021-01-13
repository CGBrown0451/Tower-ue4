// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerGameState.h"


void ATowerGameState::BeginPlay()
{
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
}

void ATowerGameState::StartGame()
{
	GameState = GameState_InGame;
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "GO!!");
}

void ATowerGameState::EndGame()
{
	GameState = GameState_PostGame;
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
}

void ATowerGameState::Tick(float DeltaSeconds)
{
	switch(GameState)
	{
	case GameState_InGame:
		GameTime += DeltaSeconds;
		break;
	}
}
