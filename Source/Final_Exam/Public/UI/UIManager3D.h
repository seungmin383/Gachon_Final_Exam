#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UIManager3D.generated.h"

class ACPlayer;
class AHeartUIActor;

UCLASS()
class FINAL_EXAM_API AUIManager3D : public AActor
{
	GENERATED_BODY()

public:
	AUIManager3D();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<AHeartUIActor> HeartClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "1.0"))
	float CameraDistance{ 250.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float RightOffset{ 200.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float UpOffset{ 90.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI", meta = (ClampMin = "0.0"))
	float HeartSpacing{ 35.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector HeartScale{ 0.25f, 0.25f, 0.25f };

private:
	void FindPlayer();
	void RefreshHearts();
	void UpdateHeartTransforms();
	void DestroyHearts();

	UPROPERTY()
	ACPlayer* Player{ nullptr };

	UPROPERTY()
	TArray<AHeartUIActor*> Hearts;

	int32 DisplayedHealth{ INDEX_NONE };
};
