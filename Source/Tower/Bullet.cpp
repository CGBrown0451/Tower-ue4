// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/KismetSystemLibrary.h"
#include <string>


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	ColSphere->InitSphereRadius(ColSphereRadius);
	ColSphere->BodyInstance.SetCollisionProfileName("Projectile");
	ColSphere->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ColSphere;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	

}

void ABullet::OnConstruction(const FTransform& Transform)
{
	FVector position = Transform.GetLocation();
	SetActorLocation(FVector(position.X, position.Y, 60.0f));
}


// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Oof"));
	
}