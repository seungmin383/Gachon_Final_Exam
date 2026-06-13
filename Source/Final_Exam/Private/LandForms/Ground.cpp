
#include "LandForms/Ground.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AGround::AGround()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	GroundMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GroundMesh"));
	GroundMesh->SetupAttachment(Root);
	GroundMesh->SetMobility(EComponentMobility::Movable);
}

void AGround::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGround::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGround::RotateGround(FVector2D axis, float DeltaTime)
{
	if (DeltaTime <= 0.0f || GroundRadius <= KINDA_SMALL_NUMBER)
	{
		return;
	}

	const FVector2D Input = axis.GetClampedToMaxSize(1.0f);
	if (Input.IsNearlyZero())
	{
		return;
	}

	const FVector MoveDirection(Input.Y, Input.X, 0.0f);
	const FVector RotationAxis = FVector::CrossProduct(MoveDirection, FVector::UpVector).GetSafeNormal();
	const float RotationAmountRadians = (MoveSpeed / GroundRadius) * Input.Size() * DeltaTime;

	GroundMesh->AddWorldRotation(FQuat(RotationAxis, RotationAmountRadians));
}
