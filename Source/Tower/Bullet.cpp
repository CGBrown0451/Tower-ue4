// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "Kismet/KismetSystemLibrary.h"
#include <string>
#include "DamageTaker.h"


// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ColSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCol"));
	ColSphere->InitSphereRadius(ColSphereRadius);
	ColSphere->SetupAttachment(RootComponent);
	ColSphere->SetCollisionProfileName("Projectile");
	ColSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ColSphere->SetNotifyRigidBodyCollision(true);
	ColSphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	ColSphere->SetGenerateOverlapEvents(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ColSphere;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bRotationRemainsVertical = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cone(TEXT("/Engine/BasicShapes/Cone.Cone"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	Mesh->SetStaticMesh(Cone.Object);
	Mesh->SetupAttachment(ColSphere);
	Mesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeScale3D(FVector(0.5f));
	

}

void ABullet::OnConstruction(const FTransform& Transform)
{
	FVector position = Transform.GetLocation();
	SetActorLocation(FVector(position.X, position.Y, 60.0f));
}

void ABullet::InitialiseObject(FTransform Transform)
{
	SetActorTransform(Transform);
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Oef");
	IDamageTaker* dmg = Cast<IDamageTaker>(OtherActor);

	if (dmg)
	{
		dmg->Execute_TakeDamage(OtherActor);
	}
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


