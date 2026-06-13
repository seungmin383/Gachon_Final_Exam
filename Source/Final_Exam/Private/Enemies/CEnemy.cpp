// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/CEnemy.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "CPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LandForms/Ground.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerAttack.h"


// Sets default values
ACEnemy::ACEnemy()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = m_pBoxComponent;
	
	
	m_pBoxComponent->SetCollisionProfileName(TEXT("OverlapAll"));
	
	m_pBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	
	m_pBodyMesh->SetupAttachment(m_pBoxComponent);
	
	// Body Mesh에 Cube Static Mesh 데이터 로드해서 할당하기
	// FClassFinder도 있음
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	
	if (tempMesh.Succeeded())
	{
		m_pBodyMesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ACEnemy::BeginPlay()
{
	Super::BeginPlay();

	PlaceOnRandomPlanetSurface();
	m_pBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ACEnemy::OnHit);
}

// Called every frame
void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemy::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsDead || OtherActor == nullptr || OtherActor == this || OtherActor == Planet || OtherActor->IsA<ACEnemy>())
	{
		return;
	}

	if (APlayerAttack* Attack = Cast<APlayerAttack>(OtherActor))
	{
		if (Attack->IsAttackActive())
		{
			Die();
		}
		return;
	}

	if (ACPlayer* Player = Cast<ACPlayer>(OtherActor))
	{
		if (OtherComp == Player->m_pBoxComponent)
		{
			if (Player->IsAttackActive())
			{
				Die();
			}
			else
			{
				Player->TakeEnemyDamage(1);
			}
		}
	}
}

void ACEnemy::PlaceOnRandomPlanetSurface()
{
	if (Planet == nullptr)
	{
		Planet = Cast<AGround>(UGameplayStatics::GetActorOfClass(GetWorld(), AGround::StaticClass()));
	}

	if (Planet == nullptr || Planet->GroundMesh == nullptr)
	{
		return;
	}

	const FVector PlanetCenter = Planet->GroundMesh->GetComponentLocation();
	const APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	
	const FVector PlayerNormal = Player != nullptr
		? (Player->GetActorLocation() - PlanetCenter).GetSafeNormal()
		: FVector::UpVector;

	const float MinAngle = FMath::Min(MinSpawnAngle, SpawnAngle);
	const float AngleRadians = FMath::DegreesToRadians(FMath::FRandRange(MinAngle, SpawnAngle));
	const FVector RandomRotationAxis = FVector::CrossProduct(PlayerNormal, FMath::VRand()).GetSafeNormal();
	const FVector SurfaceNormal = RandomRotationAxis.IsNearlyZero()
		? PlayerNormal
		: FQuat(RandomRotationAxis, AngleRadians).RotateVector(PlayerNormal).GetSafeNormal();

	const float MeshRadius = Planet->GroundMesh->Bounds.SphereRadius;
	const float PlanetRadius = MeshRadius > KINDA_SMALL_NUMBER
		? MeshRadius
		: Planet->GroundRadius;
	const float EnemyHalfHeight = m_pBoxComponent->GetScaledBoxExtent().Z;
	const float SurfaceDistance = PlanetRadius + EnemyHalfHeight + SurfaceOffset;
	const FVector SurfaceLocation = PlanetCenter + SurfaceNormal * SurfaceDistance;

	FVector ForwardDirection = FVector::CrossProduct(FVector::UpVector, SurfaceNormal).GetSafeNormal();
	if (ForwardDirection.IsNearlyZero())
	{
		ForwardDirection = FVector::CrossProduct(FVector::RightVector, SurfaceNormal).GetSafeNormal();
	}

	const FRotator SurfaceRotation =
		FRotationMatrix::MakeFromZX(SurfaceNormal, ForwardDirection).Rotator();

	SetActorLocationAndRotation(SurfaceLocation, SurfaceRotation);
	AttachToComponent(Planet->GroundMesh, FAttachmentTransformRules::KeepWorldTransform);
}

void ACEnemy::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	if (DeathEffect != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	m_pBoxComponent->SetGenerateOverlapEvents(false);
	Destroy();
}
