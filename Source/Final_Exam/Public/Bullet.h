// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class FINAL_EXAM_API ABullet : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:
	UPROPERTY(VisibleAnywhere, Category=ShootComponent)
	class UBoxComponent* m_pBoxComponent;
	UPROPERTY(VisibleAnywhere, Category=ShootComponent)
	class UStaticMeshComponent* m_pBodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status"); // 자동 메모리 관리
	float m_fSpeed {500.f};
	
};
