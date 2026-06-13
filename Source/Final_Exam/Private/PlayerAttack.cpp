// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttack.h"
#include "Components/SphereComponent.h"
#include "Enemies/CEnemy.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"

APlayerAttack::APlayerAttack()
{
	PrimaryActorTick.bCanEverTick = false;

	AttackCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AttackCollision"));
	SetRootComponent(AttackCollision);

	AttackCollision->SetSphereRadius(AttackCollisionRadius);
	AttackCollision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerAttack::BeginPlay()
{
	Super::BeginPlay();

	AttackCollision->SetSphereRadius(AttackCollisionRadius);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

bool APlayerAttack::TryAttack(EPlayerAttackType eAttackType)
{
	if (!bCanAttack || bAttackActive || eAttackType == EPlayerAttackType::END)
	{
		return false;
	}

	UNiagaraComponent* AttackEffect = SpawnAttackEffect(eAttackType);
	if (AttackEffect == nullptr)
	{
		return false;
	}

	bCanAttack = false;
	bAttackActive = true;
	AttackCollision->SetSphereRadius(AttackCollisionRadius, true);
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	KillEnemiesInRange();
	AttackEffect->OnSystemFinished.AddDynamic(this, &APlayerAttack::OnAttackEffectFinished);

	GetWorldTimerManager().SetTimer(
		AttackCooldownTimer,
		this,
		&APlayerAttack::ResetAttackCooldown,
		AttackInterval,
		false
	);

	return true;
}

bool APlayerAttack::CanAttack() const
{
	return bCanAttack;
}

bool APlayerAttack::IsAttackActive() const
{
	return bAttackActive;
}

void APlayerAttack::KillEnemiesInRange()
{
	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACEnemy::StaticClass(), Enemies);

	const FVector AttackLocation = GetActorLocation();
	const float AttackRadiusSquared = FMath::Square(AttackCollisionRadius);

	for (AActor* EnemyActor : Enemies)
	{
		ACEnemy* Enemy = Cast<ACEnemy>(EnemyActor);
		if (Enemy != nullptr && FVector::DistSquared(AttackLocation, Enemy->GetActorLocation()) <= AttackRadiusSquared)
		{
			Enemy->Die();
		}
	}
}

void APlayerAttack::FinishAttack()
{
	bAttackActive = false;
	AttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void APlayerAttack::ResetAttackCooldown()
{
	bCanAttack = true;
}

void APlayerAttack::OnAttackEffectFinished(UNiagaraComponent* FinishedComponent)
{
	FinishAttack();
}

UNiagaraComponent* APlayerAttack::SpawnAttackEffect(EPlayerAttackType eAttackType)
{
	UNiagaraSystem* Effect = nullptr;

	switch (eAttackType)
	{
	case EPlayerAttackType::FIRE:
		Effect = FireEffect;
		break;

	case EPlayerAttackType::MANA:
		Effect = ManaEffect;
		break;

	case EPlayerAttackType::LIGHTNING:
		Effect = LightningEffect;
		break;

	default:
		return nullptr;
	}

	if (Effect == nullptr)
	{
		return nullptr;
	}

	return UNiagaraFunctionLibrary::SpawnSystemAttached(
		Effect,
		AttackCollision,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true
	);
}
