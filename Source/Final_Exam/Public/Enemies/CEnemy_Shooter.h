// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CEnemy_Shooter.generated.h"

UCLASS()
class FINAL_EXAM_API ACEnemy_Shooter : public AActor
{
	GENERATED_BODY()
public:
	ACEnemy_Shooter();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;
	
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
};

