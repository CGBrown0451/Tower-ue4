// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "DamageManager.h"
#include "Damage.h"
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
	float Speed = 1600.0f;
	UPROPERTY(EditAnywhere)
	int NumBounces = 4;
	UPROPERTY(EditAnywhere)
	float ColSphereRadius = 32.0f;

	bool bHasBounced = false;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY()
	USphereComponent* ColSphere;
	UPROPERTY()
	UStaticMeshComponent* Mesh;

	UPROPERTY()
	FDamageStats Stats;

	UPROPERTY()
	AActor* Parent;

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnConstruction(const FTransform& Transform) override;

	void InitialiseStats(FDamageStats DamageStats, AActor* Parent);

	FHitResult HitResult;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity);

};






