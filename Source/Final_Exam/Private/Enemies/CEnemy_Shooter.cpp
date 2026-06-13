// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemies/CEnemy_Shooter.h"
#include "Components/BoxComponent.h"


// Sets default values
ACEnemy_Shooter::ACEnemy_Shooter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACEnemy_Shooter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACEnemy_Shooter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACEnemy_Shooter::OnHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

