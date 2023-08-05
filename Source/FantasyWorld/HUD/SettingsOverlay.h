// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOverlay.generated.h"

class UMyGameInstance;
class USlider;
class UButton;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API USettingsOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct();

	UFUNCTION()
	void HandleBGMVolumeChanged(float value);

	UFUNCTION()
	void HandleEffectSoundVolumeChanged(float value);

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void HideContinueButton();

protected:


private:

	UPROPERTY()
	UMyGameInstance* GameInstance;

	UPROPERTY(meta = (BindWidget))
	USlider* BGMVolume;

	UPROPERTY(meta = (BindWidget))
	USlider* EffectSoundVolume;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
};
