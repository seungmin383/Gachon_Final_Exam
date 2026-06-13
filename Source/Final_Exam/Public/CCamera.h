// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CCamera.generated.h"

UCLASS()
class FINAL_EXAM_API ACCamera : public ACameraActor
{
	GENERATED_BODY()

public:
	ACCamera();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	void FollowCameraPoint(float DeltaTime);
	void LookPlayer(float DeltaTime);
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	APawn* Player{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	USceneComponent* CameraPoint{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	FVector Offset{ -600.0f, 0.0f, 300.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	bool bUseSmoothFollow{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float InterpSpeed{ 5.0f };
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float RotateInterpSpeed{ 8.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	bool bLookAtTarget{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float BlendTime{ 0.0f };

private:
	FVector m_vNewLocation{ 0.f, 0.0f, 0.f };
	
};
