// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "FantasyWorld/Character/Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


// 애니메이션 초기화를 수행하는 함수
void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();  // 부모 클래스의 초기화 함수를 호출

	// 오너 Pawn이 AEnemy 타입인 경우 Enemy 변수에 할당
	Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (Enemy) {  // Enemy가 null이 아니라면
		EnemyMovement = Enemy->GetCharacterMovement();  // 캐릭터의 움직임 컴포넌트를 가져와서 EnemyMovement에 저장
	}
}

// 프레임마다 애니메이션 상태를 업데이트하는 함수
void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);  // 부모 클래스의 업데이트 함수를 호출

	if (EnemyMovement) {  // EnemyMovement가 null이 아니라면

		// 캐릭터의 지면 속도를 계산하여 GroundSpeed에 저장
		GroundSpeed = UKismetMathLibrary::VSizeXY(EnemyMovement->Velocity);

		// 캐릭터가 떨어지고 있는지 확인하여 IsFalling에 저장
		IsFalling = EnemyMovement->IsFalling();

		// 적의 현재 상태(추적, 공격 등)를 ActionState에 저장
		ActionState = Enemy->GetEnemyState();

		// 적의 죽음 포즈를 DeathPose에 저장
		DeathPose = Enemy->GetDeathPose();
	}
}
