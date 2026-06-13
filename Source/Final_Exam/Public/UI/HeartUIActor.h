#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeartUIActor.generated.h"

class USceneComponent;
class UAnimSequence;
class USkeletalMeshComponent;

UCLASS()
class FINAL_EXAM_API AHeartUIActor : public AActor
{
	GENERATED_BODY()

public:
	AHeartUIActor();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	USceneComponent* Root{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	USkeletalMeshComponent* HeartMesh{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	UAnimSequence* HeartAnimation{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	bool bLoopAnimation{ true };
};
