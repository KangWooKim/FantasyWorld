// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseZombieAnimInstance.h"
#include "EliteZombie.h"
#include "EliteZombieAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLOODYGROUND_API UEliteZombieAnimInstance : public UBaseZombieAnimInstance
{
	GENERATED_BODY()

public:

	// �ִϸ��̼� ������Ʈ �� ȣ��Ǵ� �Լ�
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EZombieInjuryState ZombieInjuryState;

private:


	
};
