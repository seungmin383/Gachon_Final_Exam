#include "UI/UIManager3D.h"

#include "CPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HeartUIActor.h"

AUIManager3D::AUIManager3D()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AUIManager3D::BeginPlay()
{
	Super::BeginPlay();

	FindPlayer();
	RefreshHearts();
}

void AUIManager3D::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	DestroyHearts();

	Super::EndPlay(EndPlayReason);
}

void AUIManager3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(Player))
	{
		FindPlayer();
	}

	RefreshHearts();
	UpdateHeartTransforms();
}

void AUIManager3D::FindPlayer()
{
	Player = Cast<ACPlayer>(UGameplayStatics::GetPlayerPawn(this, 0));
}

void AUIManager3D::RefreshHearts()
{
	const int32 TargetHealth = IsValid(Player) ? FMath::Max(Player->CurrentHealth, 0) : 0;
	if (TargetHealth == DisplayedHealth)
	{
		return;
	}

	if (HeartClass == nullptr)
	{
		HeartClass = AHeartUIActor::StaticClass();
	}

	while (Hearts.Num() < TargetHealth)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Owner = this;

		AHeartUIActor* Heart = GetWorld()->SpawnActor<AHeartUIActor>(
			HeartClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParameters
		);

		if (Heart == nullptr)
		{
			break;
		}

		Heart->SetActorScale3D(HeartScale);
		Hearts.Add(Heart);
	}

	while (Hearts.Num() > TargetHealth)
	{
		AHeartUIActor* Heart = Hearts.Pop();
		if (IsValid(Heart))
		{
			Heart->Destroy();
		}
	}

	DisplayedHealth = Hearts.Num();
}

void AUIManager3D::UpdateHeartTransforms()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	if (PlayerController == nullptr || PlayerController->PlayerCameraManager == nullptr)
	{
		return;
	}

	const FVector CameraLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
	const FRotator CameraRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
	const FVector Forward = CameraRotation.Vector();
	const FVector Right = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y);
	const FVector Up = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Z);

	for (int32 Index = 0; Index < Hearts.Num(); ++Index)
	{
		AHeartUIActor* Heart = Hearts[Index];
		if (!IsValid(Heart))
		{
			continue;
		}

		const float HorizontalOffset = RightOffset - (HeartSpacing * Index);
		const FVector HeartLocation =
			CameraLocation
			+ Forward * CameraDistance
			+ Right * HorizontalOffset
			+ Up * UpOffset;

		Heart->SetActorLocationAndRotation(HeartLocation, CameraRotation);
		Heart->SetActorScale3D(HeartScale);
	}
}

void AUIManager3D::DestroyHearts()
{
	for (AHeartUIActor* Heart : Hearts)
	{
		if (IsValid(Heart))
		{
			Heart->Destroy();
		}
	}

	Hearts.Empty();
	DisplayedHealth = INDEX_NONE;
}
