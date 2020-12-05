// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "JamesChar.generated.h"
/*
 * A class made for testing. Not important.
 */
UCLASS()
class TOWER_API AJamesChar : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AJamesChar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FVector2D MoveVector;
	
	//UFUNCTION()
		//void MoveX(float mag);
	//UFUNCTION()
		//void MoveY(float mag);
};
