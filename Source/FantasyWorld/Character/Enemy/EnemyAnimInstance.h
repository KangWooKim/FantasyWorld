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
	// �ִϸ��̼� �ʱ�ȭ �Լ�, �θ� Ŭ�������� �������̵�
	virtual void NativeInitializeAnimation() override;

	// �ִϸ��̼� ������Ʈ �Լ�, �θ� Ŭ�������� �������̵�
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	// �� Ŭ������ ���� ������ ������ �ִ� AEnemy ��ü
	UPROPERTY(BlueprintReadOnly)
		AEnemy* Enemy;

	// �� ĳ������ �������� ����ϴ� ������Ʈ
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		UCharacterMovementComponent* EnemyMovement;

	// �� ĳ������ ���鿡���� �ӵ�
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		float GroundSpeed;

	// �� ĳ���Ͱ� �������� �ִ��� ����
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		bool IsFalling;

	// �� ĳ������ ���� ���� (���� ��, ���� �� ��)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		EEnemyState ActionState;

	// �� ĳ���Ͱ� �׾��� ���� ���� (EDeathPose enum ���� ���)
	UPROPERTY(BlueprintReadOnly, Category = Movement)
		TEnumAsByte<EDeathPose> DeathPose;


protected:

private:


};
