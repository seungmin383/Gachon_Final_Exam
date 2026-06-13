// Fill out your copyright notice in the Description page of Project Settings.


#include "Final_Exam/Public/CPlayer.h"

#include "Components/BoxComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LandForms/Ground.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerAttack.h"


ACPlayer::ACPlayer()
{
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

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	m_pGround = Cast<AGround>(UGameplayStatics::GetActorOfClass(GetWorld(), AGround::StaticClass())
	);

	CreatePlayerAttack();
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_pGround != nullptr)
	{
		m_pGround->RotateGround(m_vMoveInput, DeltaTime);
	}

	UpdatePlayerLean(DeltaTime);
}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	// 사용자 입력 매핑 에셋 등록
	auto pc = Cast<APlayerController>(Controller);
	if (pc)
	{
		// 서브 시스템을 만들어서 싱글톤을 대체(?) // 온라인 서브시스템
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
	
		if (subsystem)
		{
			subsystem->AddMappingContext(m_pIMC_Shoot, 0);
		}
	}
	
	// 처리 함수와 입력 액션은 바인딩
	auto playerInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (playerInput)
	{
		playerInput->BindAction(m_pIA_Move, ETriggerEvent::Triggered, this, &ACPlayer::MovePlayer);
		playerInput->BindAction(m_pIA_Move, ETriggerEvent::Completed, this, &ACPlayer::StopMovePlayer);
		playerInput->BindAction(m_pIA_Move, ETriggerEvent::Canceled, this, &ACPlayer::StopMovePlayer);

		if (m_pIA_Attack != nullptr)
		{
			playerInput->BindAction(m_pIA_Attack, ETriggerEvent::Started, this, &ACPlayer::AttackPlayer);
		}
	}
	
}

void ACPlayer::MovePlayer(const FInputActionValue& value)
{
	FVector2D Input = value.Get<FVector2D>();
	m_vMoveInput = Input;
}

void ACPlayer::StopMovePlayer(const FInputActionValue& value)
{
	m_vMoveInput = FVector2D::ZeroVector;
}

void ACPlayer::AttackPlayer(const FInputActionValue& value)
{
	if (PlayerAttack == nullptr)
	{
		return;
	}

	const int32 AttackInput = FMath::RoundToInt(value.Get<float>());

	switch (AttackInput)
	{
	case 1:
		PlayerAttack->TryAttack(EPlayerAttackType::FIRE);
		break;

	case 2:
		PlayerAttack->TryAttack(EPlayerAttackType::MANA);
		break;

	case 3:
		PlayerAttack->TryAttack(EPlayerAttackType::LIGHTNING);
		break;

	default:
		break;
	}
}

void ACPlayer::TakeEnemyDamage(int32 DamageAmount)
{
	if (bIsDead || DamageAmount <= 0 || IsAttackActive())
	{
		return;
	}

	CurrentHealth = FMath::Max(CurrentHealth - DamageAmount, 0);

	if (CurrentHealth <= 0)
	{
		Die();
	}
}

bool ACPlayer::IsAttackActive() const
{
	return PlayerAttack != nullptr && PlayerAttack->IsAttackActive();
}

void ACPlayer::CreatePlayerAttack()
{
	if (PlayerAttackClass == nullptr)
	{
		PlayerAttackClass = APlayerAttack::StaticClass();
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;

	PlayerAttack = GetWorld()->SpawnActor<APlayerAttack>(
		PlayerAttackClass,
		GetActorLocation(),
		GetActorRotation(),
		SpawnParameters
	);

	if (PlayerAttack != nullptr)
	{
		PlayerAttack->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ACPlayer::Die()
{
	if (bIsDead)
	{
		return;
	}

	bIsDead = true;

	if (DeathEffect != nullptr)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(),
			DeathEffect,
			GetActorLocation(),
			GetActorRotation()
		);
	}

	if (PlayerAttack != nullptr)
	{
		PlayerAttack->Destroy();
	}

	Destroy();
}

void ACPlayer::UpdatePlayerLean(float DeltaTime)
{
	if (m_pBodyMesh == nullptr)
	{
		return;
	}

	const float TargetRoll = m_vMoveInput.X * MaxLeanAngle;
	const FRotator CurrentRotation = m_pBodyMesh->GetRelativeRotation();
	const float NewRoll = FMath::FInterpTo(CurrentRotation.Roll, TargetRoll, DeltaTime, LeanInterpSpeed);

	m_pBodyMesh->SetRelativeRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, NewRoll));
}
