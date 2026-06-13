// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enums/GameEnums.h"
#include "PlayerAttack.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;

UCLASS()
class FINAL_EXAM_API APlayerAttack : public AActor
{
	GENERATED_BODY()

public:
	APlayerAttack();

protected:
	virtual void BeginPlay() override;

public:
	bool TryAttack(EPlayerAttackType eAttackType);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UNiagaraSystem* FireEffect{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UNiagaraSystem* ManaEffect{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UNiagaraSystem* LightningEffect{ nullptr };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	USphereComponent* AttackCollision{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "0.01"))
	float AttackInterval{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta = (ClampMin = "1.0"))
	float AttackCollisionRadius{ 200.0f };

	UFUNCTION(BlueprintPure, Category = "Attack")
	bool CanAttack() const;

	UFUNCTION(BlueprintPure, Category = "Attack")
	bool IsAttackActive() const;

private:
	void KillEnemiesInRange();
	void FinishAttack();
	void ResetAttackCooldown();
	UNiagaraComponent* SpawnAttackEffect(EPlayerAttackType eAttackType);

	UFUNCTION()
	void OnAttackEffectFinished(UNiagaraComponent* FinishedComponent);
	
	bool bCanAttack{ true };
	bool bAttackActive{ false };
	FTimerHandle AttackCooldownTimer;
};
