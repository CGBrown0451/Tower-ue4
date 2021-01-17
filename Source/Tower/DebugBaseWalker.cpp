// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugBaseWalker.h"
/*
 *The derived class of BaseWalker that is used to visualise the Character's facing direction and give an idea of it's hitbox.
 */
ADebugBaseWalker::ADebugBaseWalker(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer.SetDefaultSubobjectClass<UMyCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cone(TEXT("/Engine/BasicShapes/Cone.Cone"));

	ConeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	ConeMesh->SetupAttachment(RootComponent);
	ConeMesh->SetStaticMesh(Cone.Object);
	ConeMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	ConeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WobbleComponent = ConeMesh;
	
}