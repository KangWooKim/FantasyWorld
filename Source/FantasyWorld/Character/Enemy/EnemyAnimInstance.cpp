// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "FantasyWorld/Character/Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// �ִϸ��̼� �ʱ�ȭ�� �����ϴ� �Լ�
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();  // �θ� Ŭ������ �ʱ�ȭ �Լ��� ȣ��

	// ���� Pawn�� AEnemy Ÿ���� ��� Enemy ������ �Ҵ�
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (Enemy) {  // Enemy�� null�� �ƴ϶��
		EnemyMovement = Enemy->GetCharacterMovement();  // ĳ������ ������ ������Ʈ�� �����ͼ� EnemyMovement�� ����
	}
}

// �����Ӹ��� �ִϸ��̼� ���¸� ������Ʈ�ϴ� �Լ�
void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);  // �θ� Ŭ������ ������Ʈ �Լ��� ȣ��

	if (EnemyMovement) {  // EnemyMovement�� null�� �ƴ϶��

		// ĳ������ ���� �ӵ��� ����Ͽ� GroundSpeed�� ����
		GroundSpeed = UKismetMathLibrary::VSizeXY(EnemyMovement->Velocity);

		// ĳ���Ͱ� �������� �ִ��� Ȯ���Ͽ� IsFalling�� ����
		IsFalling = EnemyMovement->IsFalling();

		// ���� ���� ����(����, ���� ��)�� ActionState�� ����
		ActionState = Enemy->GetEnemyState();

		// ���� ���� ��� DeathPose�� ����
		DeathPose = Enemy->GetDeathPose();
	}
}
