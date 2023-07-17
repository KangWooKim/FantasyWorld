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
	
public : 

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly)
	AEnemy* Enemy;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* EnemyMovement;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	EEnemyState ActionState;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	TEnumAsByte<EDeathPose> DeathPose;

protected:

private:


};
