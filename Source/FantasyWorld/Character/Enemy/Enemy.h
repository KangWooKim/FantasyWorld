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
	// 기본 생성자
	AEnemy();

	// Tick 메서드 오버라이드. DeltaTime은 이전 프레임과 현재 프레임 사이의 시간 간격
	virtual void Tick(float DeltaTime) override;

	// 데미지를 처리하는 메서드. DamageAmount는 데미지 양, DamageEvent는 데미지 유형, EventInstigator는 데미지를 입힌 컨트롤러, DamageCauser는 데미지를 입힌 액터
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 액터가 파괴될 때 호출되는 메서드
	virtual void Destroyed() override;

	/** <IHitInterface> */
	// 충돌 이벤트 처리. ImpactPoint는 충돌 지점, Hitter는 충돌을 일으킨 액터
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	/** </IHitInterface> */

	// 적 상태를 가져오는 메서드
	FORCEINLINE EEnemyState GetEnemyState() { return EnemyState; }

	// 순찰 대상 설정을 위한 프로퍼티
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		AActor* PatrolTarget;

	// 여러 순찰 대상 설정을 위한 프로퍼티 배열
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
		TArray<AActor*> PatrolTargets;

protected:
	// BeginPlay 메서드 오버라이드. 게임 시작 시 호출
	virtual void BeginPlay() override;

	// Die 메서드 구현
	virtual void Die_Implementation() override;

	// 소울(영혼)을 생성하는 메서드
	void SpawnSoul();

	// Attack 메서드 오버라이드. 적이 공격할 때 호출
	virtual void Attack() override;

	// 공격 가능한 상태인지 확인하는 메서드
	virtual bool CanAttack() override;

	// 공격이 끝났을 때 호출되는 메서드
	virtual void AttackEnd() override;

	// 데미지 처리를 위한 메서드. DamageAmount는 데미지 양
	virtual void HandleDamage(float DamageAmount) override;

	// 적의 상태를 저장하는 프로퍼티. 초기값은 '순찰 중'
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
		EEnemyState EnemyState = EEnemyState::EES_Patrolling;




private:

	/** AI Behavior */
// 적 AI를 초기화하는 함수
	void InitializeEnemy();

	// 순찰 대상을 확인하는 함수
	void CheckPatrolTarget();

	// 전투 대상을 확인하는 함수
	void CheckCombatTarget();

	// 순찰 타이머가 끝났을 때 호출되는 함수
	void PatrolTimerFinished();

	// 체력바를 숨기는 함수
	void HideHealthBar();

	// 체력바를 보이게 하는 함수
	void ShowHealthBar();

	// 적의 관심을 잃게 하는 함수
	void LoseInterest();

	// 순찰을 시작하는 함수
	void StartPatrolling();

	// 대상을 추적하는 함수
	void ChaseTarget();

	// 공격 반경 외에 있는지 확인하는 함수
	bool IsOutsideCombatRadius();

	// 공격 반경 외에 있는지 확인하는 함수
	bool IsOutsideAttackRadius();

	// 공격 반경 내에 있는지 확인하는 함수
	bool IsInsideAttackRadius();

	// 현재 추적 중인지 확인하는 함수
	bool IsChasing();

	// 현재 공격 중인지 확인하는 함수
	bool IsAttacking();

	// 현재 죽었는지 확인하는 함수
	bool IsDead();

	// 현재 전투 중인지 확인하는 함수
	bool IsEngaged();

	// 순찰 타이머를 초기화하는 함수
	void ClearPatrolTimer();

	// 공격 타이머를 시작하는 함수
	void StartAttackTimer();

	// 공격 타이머를 초기화하는 함수
	void ClearAttackTimer();

	// 대상과의 거리가 지정한 반경 내에 있는지 확인하는 함수
	bool InTargetRange(AActor* Target, double Radius);

	// 대상까지 이동하는 함수
	void MoveToTarget(AActor* Target);

	// 순찰 대상을 선택하는 함수
	AActor* ChoosePatrolTarget();

	// 기본 무기를 생성하는 함수
	void SpawnDefaultWeapon();

	// 폰이 보이면 호출되는 함수. UPawnSensingComponent에서 사용
	UFUNCTION()
		void PawnSeen(APawn* SeenPawn);

	// 체력바 위젯 컴포넌트
	UPROPERTY(VisibleAnywhere)
		UHealthBarComponent* HealthBarWidget;

	// 폰 감지 컴포넌트
	UPROPERTY(VisibleAnywhere)
		UPawnSensingComponent* PawnSensing;

	// 사용할 무기 클래스
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	// 전투 반경
	UPROPERTY(EditAnywhere, Category = Combat)
		double CombatRadius = 500.f;

	// 공격 반경
	UPROPERTY(EditAnywhere, Category = Combat)
		double AttackRadius = 150.f;

	// 목표 수용 반경
	UPROPERTY(EditAnywhere, Category = Combat)
		double AcceptanceRadius = 50.f;

	// 적 AI 컨트롤러
	UPROPERTY()
		class AAIController* EnemyController;

	// 추적 사운드
	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* ChaseSound;

	// 순찰 반경
	UPROPERTY(EditAnywhere)
		double PatrolRadius = 200.f;

	// 순찰 타이머 핸들
	FTimerHandle PatrolTimer;

	// 순찰 대기 시간 최소값
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMin = 5.f;

	// 순찰 대기 시간 최대값
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
		float PatrolWaitMax = 10.f;

	// 순찰 속도
	UPROPERTY(EditAnywhere, Category = Combat)
		float PatrollingSpeed = 125.f;

	// 공격 타이머 핸들
	FTimerHandle AttackTimer;

	// 최소 공격 시간
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMin = 0.5f;

	// 최대 공격 시간
	UPROPERTY(EditAnywhere, Category = Combat)
		float AttackMax = 1.f;

	// 추적 속도
	UPROPERTY(EditAnywhere, Category = Combat)
		float ChasingSpeed = 300.f;

	// 죽은 후 사라지는 시간
	UPROPERTY(EditAnywhere, Category = Combat)
		float DeathLifeSpan = 3.f;

	// 생성할 '영혼' 클래스
	UPROPERTY(EditAnywhere, Category = Combat)
		TSubclassOf<class ASoul> SoulClass;


};
