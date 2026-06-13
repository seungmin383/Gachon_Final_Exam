// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/BoxComponent.h"


// Sets default values
ABullet::ABullet()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	m_pBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = m_pBoxComponent;
	
	
	m_pBoxComponent->SetCollisionProfileName(TEXT("BlockAll"));
	
	m_pBodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMeshComponent"));
	
	m_pBodyMesh->SetupAttachment(m_pBoxComponent);
	
	// Body Mesh에 Cube Static Mesh 데이터 로드해서 할당하기
	// FClassFinder도 있음
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	
	if (tempMesh.Succeeded())
	{
		m_pBodyMesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(GetActorLocation() + FVector::UpVector * m_fSpeed * DeltaTime);
}

