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
	bBlockInput = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
}

void ABasePlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	Walker = Cast<ABaseWalker>(InPawn);
	
	SetViewTargetWithBlend(Walker, 0.0f, VTBlend_Linear);
	Walker->GetCharacterMovement()->SetActive(true, true);

	
}



FVector ABasePlayerController::LookDirectionFromWorldPoint(FVector WorldPoint)
{
	WorldPoint.Z = 60.0f;
	FVector point = WorldPoint - Walker->GetActorLocation();
	point.Normalize();
	return point;
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

void ABasePlayerController::StartAim()
{
	bIsAiming = true;
}

void ABasePlayerController::EndAim()
{
	bIsAiming = false;
}

void ABasePlayerController::Aim()
{
	TPair<FVector,bool> Look = DetermineLookVector();
	if(Look.Value)
	{
		Walker->GetMyMovementComponent()->LookInDirection(Look.Key,0.0f);
	}else
	{
		Walker->GetMyMovementComponent()->LookAtPoint(Look.Key,0.0f);
	}
}

TPair<FVector,bool> ABasePlayerController::DetermineLookVector()
{
	TPair<FVector,bool> returnvalue;
	if (AimDir.Size() > 0.1f)
	{
		returnvalue.Key = FVector(AimDir,0.0f);
		returnvalue.Value = true;
		return returnvalue;
	}
	
	if (DeprojectMousePositionToWorld(MouseLocation, MouseDirection))
	{
		float MoveBy = (60.0f - MouseLocation.Z) / MouseDirection.Z;

		FVector MousePoint = MouseLocation + MouseDirection * MoveBy;

		FocusPoint = MousePoint;

		returnvalue.Key = MousePoint;
		returnvalue.Value = false;
		return returnvalue;
	}

	returnvalue.Key = FVector(MoveDir,0.0f);
	returnvalue.Value = true;
	return returnvalue;
	
}


void ABasePlayerController::StartAttack()
{
	bHasShot = true;
	DoAttack();
}

void ABasePlayerController::EndAttack()
{
	if(!bHasShot)
	{
		DoAttack();
	}else
	{
		bHasShot = false;
	}
}

void ABasePlayerController::DoAttack()
{
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "EndAttack");
	TPair<FVector,bool> Look = DetermineLookVector();
	if(Look.Value)
	{
		Walker->GetMyMovementComponent()->LookInDirection(Look.Key,1.0f);
		Walker->Attack(Look.Key);
	}else
	{
		Walker->GetMyMovementComponent()->LookAtPoint(Look.Key,1.0f);
		
		FVector LookVector = Look.Key - Walker->GetActorLocation();
		LookVector.Normalize();
		
		Walker->Attack(LookVector);
	}
}

void ABasePlayerController::DodgeInput()
{
	if (!Walker->GetMyMovementComponent()->DoDodge(MoveDir))
	{
		//TODO: Buffer a Dodge
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
	if(AimDir.Size() > 0.1f || bIsAiming)
	{
		Aim();
	}
	Walker->AddMovementInput(FVector(MoveDir,0.0f),1.0f);
}


