// Fill out your copyright notice in the Description page of Project Settings.
#include "BasePlayerController.h"
#include "Math/Vector2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <string>
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"


ABasePlayerController::ABasePlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Walker = Cast<ABaseWalker>(InPawn);
	
	SetViewTargetWithBlend(Walker, 0.0f, VTBlend_Linear);
	Walker->GetCharacterMovement()->SetActive(true, true);

	
}



FVector2D ABasePlayerController::LookDirectionFromWorldPoint(FVector WorldPoint)
{
	WorldPoint.Z = 60.0f;
	FVector point = WorldPoint - Walker->GetActorLocation();
	point.Normalize();
	return FVector2D(point.X, point.Y);
}

void ABasePlayerController::MoveX(float mag)
{
	MoveDir.X = mag;
}

void ABasePlayerController::MoveY(float mag)
{
	MoveDir.Y = mag;
}

void ABasePlayerController::AimX(float mag)
{
	if(mag != 0.0f)
	{
		AimDir.Y = mag;
	}
}

void ABasePlayerController::AimY(float mag)
{
	if(mag != 0.0f)
	{
		AimDir.X = mag;
	}
}

void ABasePlayerController::StartAim()
{
	bIsAimPressed = true;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "StartAim");
}

void ABasePlayerController::EndAim()
{
	bIsAimPressed = false;
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "EndAim");
}

void ABasePlayerController::StartAttack()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "StartAttack");
	Walker->AttackInDirection(Walker->AimDir);
}

void ABasePlayerController::EndAttack()
{
	//Negative Edge attack input, will work properly when gun system is fully implemented
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "EndAttack");
	Walker->AttackInDirection(Walker->AimDir);
}

void ABasePlayerController::DodgeInput()
{
	if (Walker->SetWalkerState(WalkerState_Dodging))
	{
		Walker->DodgeInDirection(MoveDir);
	}else
	{
		//Buffer a Dodge
	}
}

void ABasePlayerController::QuickRestart()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}


void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis(TEXT("MoveX"), this, &ABasePlayerController::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"), this, &ABasePlayerController::MoveY);

	InputComponent->BindAxis(TEXT("AimX"), this, &ABasePlayerController::AimX);
	InputComponent->BindAxis(TEXT("AimY"), this, &ABasePlayerController::AimY);

	InputComponent->BindAction(TEXT("Attack"), IE_Pressed, this, &ABasePlayerController::StartAttack);
	InputComponent->BindAction(TEXT("Attack"), IE_Released, this, &ABasePlayerController::EndAttack);

	InputComponent->BindAction("Aim", IE_Pressed, this, &ABasePlayerController::StartAim);
	InputComponent->BindAction("Aim", IE_Released, this, &ABasePlayerController::EndAim);

	InputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &ABasePlayerController::DodgeInput);

	InputComponent->BindAction("QuickRestart", IE_Pressed, this, &ABasePlayerController::QuickRestart);
	
}

void ABasePlayerController::Tick(float DeltaSeconds)
{
	bIsAiming = false;
	if (AimDir.Size() < 0.1f) {
		
		if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection)){
			float MoveBy = (60.0f - MouseLocation.Z) / MouseDirection.Z;

			FVector MousePoint = MouseLocation + MouseDirection * MoveBy;

			FocusPoint = MousePoint;

			Walker->AimDir = LookDirectionFromWorldPoint(MousePoint);

			bIsAiming = bIsAimPressed;

			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, MousePoint.ToString());
		}
	}else{
		bIsAiming = true;
		Walker->AimDir = AimDir;
		FocusPoint = Walker->GetActorLocation() + FVector(AimDir,0.0f) * 600.0f;
	}
	Walker->MoveDir = MoveDir;
	
	if(bIsAiming)
	{
		Walker->SetWalkerState(WalkerState_Aiming);
	}else
	{
		Walker->ResetWalkerState();
	}
	
}


