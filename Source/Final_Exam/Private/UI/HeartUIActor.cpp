#include "UI/HeartUIActor.h"

#include "Animation/AnimSequence.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"

AHeartUIActor::AHeartUIActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	HeartMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("HeartMesh"));
	HeartMesh->SetupAttachment(Root);
	HeartMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HeartMesh->SetCastShadow(false);
}

void AHeartUIActor::BeginPlay()
{
	Super::BeginPlay();

	if (HeartAnimation != nullptr)
	{
		HeartMesh->PlayAnimation(HeartAnimation, bLoopAnimation);
	}
}
