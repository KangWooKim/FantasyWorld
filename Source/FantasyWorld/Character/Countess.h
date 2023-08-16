// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Countess.generated.h"

class UBoxComponent;
class USoundBase;
class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ACountess : public ABaseCharacter
{
	GENERATED_BODY()
	
public : 

	ACountess();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected : 

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
	virtual void Attack() override;

	
	virtual void LethalMode() override;
	virtual void LethalModeFinish() override;

private : 

	UPROPERTY(EditAnywhere)
	int32 ConsumptionRate = 1;

	UPROPERTY()
	float TimeSinceLastDecrease = 0.0f;

	UPROPERTY(EditAnywhere)
	float NormalDamage = 20.f;

	UPROPERTY(EditAnywhere)
	float ELSDamage = 100.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* SwingEffect;

	UPROPERTY(EditAnywhere)
	float SpendHealthAmount = 10.f;
};
