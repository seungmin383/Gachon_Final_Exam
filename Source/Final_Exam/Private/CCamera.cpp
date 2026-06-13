// Fill out your copyright notice in the Description page of Project Settings.


#include "CCamera.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/PlayerController.h"


ACCamera::ACCamera()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACCamera::BeginPlay()
{
	Super::BeginPlay();
	
	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn((this), 0);
	
	if (pPlayerPawn == nullptr)
	{
		return;
	}
	
	Player = pPlayerPawn;
	
	TArray<USceneComponent*> SceneComponents;
	pPlayerPawn->GetComponents<USceneComponent>(SceneComponents);

	for (USceneComponent* Component : SceneComponents)
	{
		if (Component != nullptr && Component->GetName() == TEXT("Camera Point"))
		{
			CameraPoint = Component;
			break;
		}
	}
	
	// 실제 화면을 이 CameraActor로 전환
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController != nullptr)
	{
		PlayerController->SetViewTargetWithBlend(this, BlendTime);
	}
}

void ACCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Camera Point 따라가는 함수
	FollowCameraPoint(DeltaTime);
	LookPlayer(DeltaTime);
}

void ACCamera::FollowCameraPoint(float DeltaTime)
{
	if (nullptr == CameraPoint)
	{
		return;
	}
	
	// 1. Camera Point의 월드 위치
	FVector TargetLocation = CameraPoint->GetComponentLocation();

	// 2. 현재 카메라 위치
	FVector CurrentLocation = GetActorLocation();
	
	m_vNewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime,InterpSpeed );
	SetActorLocation(m_vNewLocation);
}

void ACCamera::LookPlayer(float DeltaTime)
{
	if (Player == nullptr)
	{
		return;
	}

	if (bLookAtTarget == false)
	{
		return;
	}

	FVector CameraLocation = GetActorLocation();

	FVector LookTargetLocation = Player->GetActorLocation();

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		CameraLocation,
		LookTargetLocation
	);

	FRotator CurrentRotation = GetActorRotation();

	FRotator NewRotation = FMath::RInterpTo(
		CurrentRotation,
		TargetRotation,
		DeltaTime,
		RotateInterpSpeed
	);

	SetActorRotation(NewRotation);
}
