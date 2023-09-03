// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Phase.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API APhase : public ABaseCharacter
{
	GENERATED_BODY()

		// APhase 클래스의 public 섹션 시작
public:

	// APhase 클래스의 기본 생성자입니다.
	APhase();

	// UInputComponent를 설정하는 가상 함수를 오버라이드합니다.
	// PlayerInputComponent: 이 클래스에 연결된 입력 컴포넌트입니다.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 데미지를 처리하는 가상 함수를 오버라이드합니다.
	// DamageAmount: 받는 데미지 양
	// DamageEvent: 데미지 이벤트 정보 (데미지 타입 등)
	// EventInstigator: 데미지를 주는 컨트롤러
	// DamageCauser: 데미지를 주는 액터
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// APhase 클래스의 protected 섹션 시작
protected:

	// 게임 시작시 한 번만 호출되는 함수입니다.
	virtual void BeginPlay() override;

	// 프레임마다 호출되는 함수입니다.
	// DeltaTime: 이전 프레임과의 시간 간격
	virtual void Tick(float DeltaTime) override;

	// 충돌 또는 공격 시 호출되는 함수의 구현입니다.
	// ImpactPoint: 공격이 또는 충돌이 발생한 지점의 위치
	// Hitter: 공격 또는 충돌을 일으킨 액터
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// 공격을 실행하는 함수입니다.
	virtual void Attack() override;

	// 필살기 모드를 활성화 또는 비활성화하는 함수입니다.
	virtual void LethalMode() override;

	// 필살기 모드를 종료하는 함수입니다.
	virtual void LethalModeFinish() override;

	// APhase 클래스의 private 섹션 시작
private:

	// 필살기 모드를 활성화할 때 소비되는 영혼의 양을 설정합니다.
	// 일반적으로 음수로 설정됩니다. (-50은 50 영혼을 소비한다는 의미)
	UPROPERTY(EditAnywhere)
		int32 SoulSpent = -50;

	// 필살기 모드 활성화 시 보여줄 쉴드 이펙트입니다.
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* ShieldEffect;

	// 공격 실행 시 나타날 이펙트입니다.
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* AttackEffect;

};
