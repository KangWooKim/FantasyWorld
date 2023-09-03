// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FantasyWorld/Character/CharacterTypes.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;
class UCharacterMovementComponent;


/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// 애니메이션 초기화 함수, 부모 클래스에서 오버라이딩
	virtual void NativeInitializeAnimation() override;

	// 애니메이션 업데이트 함수, 부모 클래스에서 오버라이딩
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// 이 클래스에 대한 참조를 가지고 있는 AEnemy 객체
	UPROPERTY(BlueprintReadOnly)
		AEnemy* Enemy;

	// 적 캐릭터의 움직임을 담당하는 컴포넌트
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		UCharacterMovementComponent* EnemyMovement;

	// 적 캐릭터의 지면에서의 속도
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float GroundSpeed;

	// 적 캐릭터가 떨어지고 있는지 여부
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool IsFalling;

	// 적 캐릭터의 현재 상태 (추적 중, 공격 중 등)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		EEnemyState ActionState;

	// 적 캐릭터가 죽었을 때의 포즈 (EDeathPose enum 값을 사용)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		TEnumAsByte<EDeathPose> DeathPose;


protected:

private:


};
