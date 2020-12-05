// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "Bullet.generated.h"

/*
 * The Base Class for all projectiles in the game. Currently doesn't collide with the environment.
 */
UCLASS()
class TOWER_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();
	UPROPERTY(EditAnywhere)
	float Speed = 300.0f;
	float ColSphereRadius = 32.0f;
	
	UPROPERTY()
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY()
	USphereComponent* ColSphere;
	UPROPERTY()
	UStaticMeshComponent* Mesh;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	FHitResult HitResult;

	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};


