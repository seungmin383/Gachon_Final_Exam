// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnemy.generated.h"

class AGround;
class UNiagaraSystem;

UCLASS()
class FINAL_EXAM_API ACEnemy : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UFUNCTION()
	void OnHit( UPrimitiveComponent* OverlappedComponent, 
		AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, 
		int32 OtherBodyIndex, 
		bool bFromSweep, 
		const FHitResult & SweepResult);
public:
	UPROPERTY(VisibleAnywhere, Category=ShootComponent)
	class UBoxComponent* m_pBoxComponent;
	UPROPERTY(VisibleAnywhere, Category=ShootComponent)
	class UStaticMeshComponent* m_pBodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status"); // 자동 메모리 관리
	float m_fSpeed {500.f};

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Planet")
	AGround* Planet{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet", meta = (ClampMin = "0.0"))
	float SurfaceOffset{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float SpawnAngle{ 45.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet", meta = (ClampMin = "0.0", ClampMax = "180.0"))
	float MinSpawnAngle{ 15.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* DeathEffect{ nullptr };

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Die();

private:
	void PlaceOnRandomPlanetSurface();

	bool bIsDead{ false };
};
