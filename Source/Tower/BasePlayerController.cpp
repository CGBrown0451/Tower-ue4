// Fill out your copyright notice in the Description page of Project Settings.
#include "BasePlayerController.h"
#include "Math/Vector2D.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <string>


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
	AimDir.Y = mag;
}

void ABasePlayerController::AimY(float mag)
{
	AimDir.X = mag;
}

void ABasePlayerController::StartAttack()
{
}

void ABasePlayerController::EndAttack()
{
}

void ABasePlayerController::DodgeInput()
{
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

	InputComponent->BindAction(TEXT("Dodge"), IE_Pressed, this, &ABasePlayerController::DodgeInput);
	
}

void ABasePlayerController::Tick(float DeltaSeconds)
{
	if (AimDir.Size() < 0.1f) {
		if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
		{
			float MoveBy = (60.0f - MouseLocation.Z) / MouseDirection.Z;

			FVector MousePoint = MouseLocation + MouseDirection * MoveBy;

			Walker->AimDir = LookDirectionFromWorldPoint(MousePoint);

			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, MousePoint.ToString());

		}
	}else
	{
		Walker->AimDir = AimDir;
	}

	Walker->MoveDir = MoveDir;
	
}


