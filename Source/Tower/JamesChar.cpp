// Fill out your copyright notice in the Description page of Project Settings.


#include "JamesChar.h"

// Sets default values
AJamesChar::AJamesChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AJamesChar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJamesChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(FVector(MoveVector, 0.f), 1);

}

// Called to bind functionality to input
void AJamesChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//InputComponent->BindAxis(TEXT("MoveX"), this, &AJamesChar::MoveX);
	//InputComponent->BindAxis(TEXT("MoveY"), this, &AJamesChar::MoveY);
}


