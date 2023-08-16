// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Phase.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API APhase : public ABaseCharacter
{
	GENERATED_BODY()

public:

	APhase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	virtual void Attack() override;

	virtual void LethalMode() override;
	virtual void LethalModeFinish() override;

private:

	UPROPERTY(EditAnywhere)
	int32 SoulSpent = -50;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* ShieldEffect;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* AttackEffect;
};
