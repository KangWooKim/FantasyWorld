// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "FantasyWorld/Character/Enemy/Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Enemy = Cast<AEnemy>(TryGetPawnOwner());
	if (Enemy) {
		EnemyMovement = Enemy->GetCharacterMovement();

	}

}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (EnemyMovement) {

		GroundSpeed = UKismetMathLibrary::VSizeXY(EnemyMovement->Velocity);

		IsFalling = EnemyMovement->IsFalling();

		ActionState = Enemy->GetEnemyState();
		DeathPose = Enemy->GetDeathPose();
	}
}