// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypes.h"
#include "CountessAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UCountessAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	// 애니메이션 초기화 함수
	virtual void NativeInitializeAnimation() override;

	// 애니메이션 업데이트 함수
	// @param DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// 슬래시 캐릭터에 대한 참조
	UPROPERTY(BlueprintReadOnly)
	class ACountess* CountessCharacter;

	// 슬래시 캐릭터의 이동 컴포넌트에 대한 참조
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* CountessMovement;

	// 캐릭터의 지면 속도
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	// 캐릭터가 공중에 있는지 여부를 나타내는 플래그
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	// 캐릭터의 현재 동작 상태 (예: 공격, 회피, 죽음 등)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	// 캐릭터의 현재 죽음 포즈
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;

};
