#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ground.generated.h"

class USceneComponent;
class UStaticMeshComponent;

UCLASS()
class FINAL_EXAM_API AGround : public AActor
{
	GENERATED_BODY()

public:
	AGround();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	
public:
	void RotateGround(FVector2D axis, float DeltaTime);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground")
	USceneComponent* Root{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ground")
	UStaticMeshComponent* GroundMesh{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground", meta = (ClampMin = "0.0"))
	float MoveSpeed{ 100.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ground", meta = (ClampMin = "1.0"))
	float GroundRadius{ 1000.0f };
};
