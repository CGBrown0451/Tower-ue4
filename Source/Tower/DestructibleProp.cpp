// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructibleProp.h"

// Sets default values
ADestructibleProp::ADestructibleProp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADestructibleProp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADestructibleProp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

