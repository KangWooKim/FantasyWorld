// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "FantasyWorld/Character/CharacterTypes.h"
#include "Enemy.generated.h"

class UHealthBarComponent;
class UPawnSensingComponent;


/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AEnemy : public ABaseEnemy
{
	GENERATED_BODY()
	
public:
	// �⺻ ������
	AEnemy();

	// Tick �޼��� �������̵�. DeltaTime�� ���� �����Ӱ� ���� ������ ������ �ð� ����
	virtual void Tick(float DeltaTime) override;

	// �������� ó���ϴ� �޼���. DamageAmount�� ������ ��, DamageEvent�� ������ ����, EventInstigator�� �������� ���� ��Ʈ�ѷ�, DamageCauser�� �������� ���� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ���Ͱ� �ı��� �� ȣ��Ǵ� �޼���
	virtual void Destroyed() override;

	/** <IHitInterface> */
	// �浹 �̺�Ʈ ó��. ImpactPoint�� �浹 ����, Hitter�� �浹�� ����Ų ����
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

	// �� ���¸� �������� �޼���
	FORCEINLINE EEnemyState GetEnemyState() { return EnemyState; }

	// ���� ��� ������ ���� ������Ƽ
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	// ���� ���� ��� ������ ���� ������Ƽ �迭
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

protected:
	// BeginPlay �޼��� �������̵�. ���� ���� �� ȣ��
	virtual void BeginPlay() override;

	// Die �޼��� ����
	virtual void Die_Implementation() override;

	// �ҿ�(��ȥ)�� �����ϴ� �޼���
	void SpawnSoul();

	// Attack �޼��� �������̵�. ���� ������ �� ȣ��
	virtual void Attack() override;

	// ���� ������ �������� Ȯ���ϴ� �޼���
	virtual bool CanAttack() override;

	// ������ ������ �� ȣ��Ǵ� �޼���
	virtual void AttackEnd() override;

	// ������ ó���� ���� �޼���. DamageAmount�� ������ ��
	virtual void HandleDamage(float DamageAmount) override;

	// ���� ���¸� �����ϴ� ������Ƽ. �ʱⰪ�� '���� ��'
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;




private:

	/** AI Behavior */
// �� AI�� �ʱ�ȭ�ϴ� �Լ�
	void InitializeEnemy();

	// ���� ����� Ȯ���ϴ� �Լ�
	void CheckPatrolTarget();

	// ���� ����� Ȯ���ϴ� �Լ�
	void CheckCombatTarget();

	// ���� Ÿ�̸Ӱ� ������ �� ȣ��Ǵ� �Լ�
	void PatrolTimerFinished();

	// ü�¹ٸ� ����� �Լ�
	void HideHealthBar();

	// ü�¹ٸ� ���̰� �ϴ� �Լ�
	void ShowHealthBar();

	// ���� ������ �Ұ� �ϴ� �Լ�
	void LoseInterest();

	// ������ �����ϴ� �Լ�
	void StartPatrolling();

	// ����� �����ϴ� �Լ�
	void ChaseTarget();

	// ���� �ݰ� �ܿ� �ִ��� Ȯ���ϴ� �Լ�
	bool IsOutsideCombatRadius();

	// ���� �ݰ� �ܿ� �ִ��� Ȯ���ϴ� �Լ�
	bool IsOutsideAttackRadius();

	// ���� �ݰ� ���� �ִ��� Ȯ���ϴ� �Լ�
	bool IsInsideAttackRadius();

	// ���� ���� ������ Ȯ���ϴ� �Լ�
	bool IsChasing();

	// ���� ���� ������ Ȯ���ϴ� �Լ�
	bool IsAttacking();

	// ���� �׾����� Ȯ���ϴ� �Լ�
	bool IsDead();

	// ���� ���� ������ Ȯ���ϴ� �Լ�
	bool IsEngaged();

	// ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϴ� �Լ�
	void ClearPatrolTimer();

	// ���� Ÿ�̸Ӹ� �����ϴ� �Լ�
	void StartAttackTimer();

	// ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϴ� �Լ�
	void ClearAttackTimer();

	// ������ �Ÿ��� ������ �ݰ� ���� �ִ��� Ȯ���ϴ� �Լ�
	bool InTargetRange(AActor* Target, double Radius);

	// ������ �̵��ϴ� �Լ�
	void MoveToTarget(AActor* Target);

	// ���� ����� �����ϴ� �Լ�
	AActor* ChoosePatrolTarget();

	// �⺻ ���⸦ �����ϴ� �Լ�
	void SpawnDefaultWeapon();

	// ���� ���̸� ȣ��Ǵ� �Լ�. UPawnSensingComponent���� ���
	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

	// ü�¹� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		UHealthBarComponent* HealthBarWidget;

	// �� ���� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	// ����� ���� Ŭ����
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	// ���� �ݰ�
	UPROPERTY(EditAnywhere, Category = Combat)
		double CombatRadius = 500.f;

	// ���� �ݰ�
	UPROPERTY(EditAnywhere, Category = Combat)
		double AttackRadius = 150.f;

	// ��ǥ ���� �ݰ�
	UPROPERTY(EditAnywhere, Category = Combat)
		double AcceptanceRadius = 50.f;

	// �� AI ��Ʈ�ѷ�
	UPROPERTY()
		class AAIController* EnemyController;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* ChaseSound;

	// ���� �ݰ�
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	// ���� Ÿ�̸� �ڵ�
	FTimerHandle PatrolTimer;

	// ���� ��� �ð� �ּҰ�
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMin = 5.f;

	// ���� ��� �ð� �ִ밪
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMax = 10.f;

	// ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 125.f;

	// ���� Ÿ�̸� �ڵ�
	FTimerHandle AttackTimer;

	// �ּ� ���� �ð�
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.5f;

	// �ִ� ���� �ð�
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 1.f;

	// ���� �ӵ�
	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	// ���� �� ������� �ð�
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 3.f;

	// ������ '��ȥ' Ŭ����
	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<class ASoul> SoulClass;


};
