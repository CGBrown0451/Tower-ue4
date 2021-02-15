// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerGameState.h"

#include "Kismet/GameplayStatics.h"


void ATowerGameState::BeginPlay()
{
	
}

void ATowerGameState::StartGame()
{
	SetGameState(GameState_InGame);
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "GO!!");
}

void ATowerGameState::TogglePause()
{
	if(bHasStarted)
	{
		if(bIsPaused)
		{
			SetGameState(GameState_InGame);
			//Remove Pause Widget from the screen
		}else
		{
			SetGameState(GameState_Paused);
			//Add Pause Widget to the screen
		}
	}
}

void ATowerGameState::SetGameState(Tower_GameState NewState)
{
	switch(NewState){
		case GameState_PreGame:
			bIsPaused = true;
			bHasStarted = false;
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(),true);
		break;
		case GameState_InGame:
			bHasStarted = true;
			bIsPaused = false;
			UGameplayStatics::SetGamePaused(GetWorld(),false);
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
		break;
		case GameState_Paused:
			bHasStarted = true;
			bIsPaused = true;
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(),true);
		break;
		case GameState_PostGame:
			bHasStarted = false;
			bIsPaused = true;
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
			UGameplayStatics::SetGamePaused(GetWorld(),true);
		break;
	}
}

void ATowerGameState::EndGame(bool HasWon, int Mode)
{
	SetGameState(GameState_PostGame);
	UGameplayStatics::SetGamePaused(GetWorld(),true);
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
