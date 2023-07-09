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
	// �ִϸ��̼� �ʱ�ȭ �Լ�
	virtual void NativeInitializeAnimation() override;

	// �ִϸ��̼� ������Ʈ �Լ�
	// @param DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// ������ ĳ���Ϳ� ���� ����
	UPROPERTY(BlueprintReadOnly)
	class ACountess* CountessCharacter;

	// ������ ĳ������ �̵� ������Ʈ�� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* CountessMovement;

	// ĳ������ ���� �ӵ�
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	// ĳ���Ͱ� ���߿� �ִ��� ���θ� ��Ÿ���� �÷���
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	// ĳ������ ���� ���� ���� (��: ����, ȸ��, ���� ��)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EActionState ActionState;

	// ĳ������ ���� ���� ����
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;

};
