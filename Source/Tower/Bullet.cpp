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
	ColSphere->SetNotifyRigidBodyCollision(false);
	ColSphere->SetSimulatePhysics(false);
	ColSphere->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnOverlap);
	ColSphere->SetGenerateOverlapEvents(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = ColSphere;
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.9f;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bSweepCollision = true;
	ProjectileMovement->bRotationRemainsVertical = true;

	ProjectileMovement->OnProjectileBounce.AddDynamic(this, &ABullet::OnBounce);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cone(TEXT("/Engine/BasicShapes/Cone.Cone"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cone"));
	Mesh->SetStaticMesh(Cone.Object);
	Mesh->SetupAttachment(ColSphere);
	Mesh->SetSimulatePhysics(false);
	Mesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetRelativeScale3D(FVector(0.5f));
	
	InitialLifeSpan = 4.0f;
}

void ABullet::OnConstruction(const FTransform& Transform)
{
	FVector position = Transform.GetLocation();
	SetActorLocation(FVector(position.X, position.Y, 60.0f));
	
}

void ABullet::InitialiseStats(FDamageStats DamageStats, AActor* P)
{
	Stats = DamageStats;
	Parent = P;
}

void ABullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Stats.Direction = GetActorRotation().Vector();
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::SanitizeFloat(GetLifeSpan()));
	if (OtherActor == Parent)
	{
		if (GetLifeSpan() > 3.9f)
		{
			return;
		}
	}
	IDamageTaker* dmg = Cast<IDamageTaker>(OtherActor);

	if (dmg)
	{
		dmg->Execute_TakeDamage(OtherActor, Stats);
	}
	
	Destroy();
}

void ABullet::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	NumBounces--;
	if(NumBounces < 0)
	{
		Destroy();
	}
	float degrees = FVector::DotProduct(ImpactResult.ImpactNormal, ImpactVelocity.GetSafeNormal());
	if (FMath::Abs(degrees) > 0.9f && !bHasBounced)
	{
		Destroy();
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
