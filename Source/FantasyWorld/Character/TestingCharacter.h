// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "TestingCharacter.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATestingCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	ATestingCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;

	virtual void LethalMode() override;
	virtual void LethalModeFinish() override;

private:

	UPROPERTY()
		TArray<AActor*> LethalModeTarget;

	UPROPERTY(EditAnywhere)
		int32 SoulSpent = -50;

};
