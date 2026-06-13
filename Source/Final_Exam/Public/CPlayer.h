#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "CPlayer.generated.h"

class UBoxComponent;
class UStaticMeshComponent;
class UInputAction;
class UInputMappingContext;
class AGround;
class APlayerAttack;
class UNiagaraSystem;

UCLASS()
class FINAL_EXAM_API ACPlayer : public APawn
{
	GENERATED_BODY()

public:
	ACPlayer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = "ShootComponent")
	UBoxComponent* m_pBoxComponent{ nullptr };

	UPROPERTY(VisibleAnywhere, Category = "ShootComponent")
	UStaticMeshComponent* m_pBodyMesh{ nullptr };

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_pIA_Move{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* m_pIA_Attack{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* m_pIMC_Shoot{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Planet")
	AGround* m_pGround{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0", ClampMax = "45.0"))
	float MaxLeanAngle{ 15.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (ClampMin = "0.0"))
	float LeanInterpSpeed{ 3.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<APlayerAttack> PlayerAttackClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attack")
	APlayerAttack* PlayerAttack{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (ClampMin = "1"))
	int32 MaxHealth{ 3 };

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	int32 CurrentHealth{ 3 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	UNiagaraSystem* DeathEffect{ nullptr };

public:
	void MovePlayer(const FInputActionValue& value);
	void StopMovePlayer(const FInputActionValue& value);
	void AttackPlayer(const FInputActionValue& value);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeEnemyDamage(int32 DamageAmount = 1);

	UFUNCTION(BlueprintPure, Category = "Attack")
	bool IsAttackActive() const;

private:
	void CreatePlayerAttack();
	void Die();
	void UpdatePlayerLean(float DeltaTime);

	bool bIsDead{ false };

	FVector2D m_vMoveInput{ 0.0f, 0.0f };
};
