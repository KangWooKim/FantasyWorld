// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatOverlay.generated.h"

class UButton;
class USoundBase;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UCombatOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct();

	UFUNCTION()
	void UpdateHealthBar(float Percent);
	
	UFUNCTION()
	void UpdateStaminaBar(float Percent);

	UFUNCTION()
	void UpdateSoulText(int32 Soul);

	UFUNCTION()
	void UpdateLeftEnemyText(int32 Enemy);

protected:



private:

	UPROPERTY(EditAnywhere)
	USoundBase* ButtonClickedSound;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* SoulText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* LeftEnemyText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* StaminaBar;

};
