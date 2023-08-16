// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOverlay.generated.h"

class UMyGameInstance;
class USlider;
class UButton;
class AFantasyPlayerController;

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
	void HandleMouseSensitivitySliderChanged(float value);

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void OnContinueButtonClicked();

	UFUNCTION()
	void HideContinueButton();

	UFUNCTION()
	void OnResolutionHighButtonClicked();

	UFUNCTION()
	void OnResolutionMiddleButtonClicked();

	UFUNCTION()
	void OnResolutionLowButtonClicked();

	UFUNCTION()
	void OnFullScreenModeButtonClicked();

	UFUNCTION()
	void OnWindowModeButtonClicked();

	UPROPERTY(meta = (BindWidget))
	USlider* BGMVolume;

	UPROPERTY(meta = (BindWidget))
	USlider* EffectSoundVolume;

	UPROPERTY(meta = (BindWidget))
	USlider* MouseSensitivitySlider;

protected:


private:

	UPROPERTY()
	AFantasyPlayerController* Controller;

	UPROPERTY()
	UMyGameInstance* GameInstance;

	UPROPERTY(meta = (BindWidget))
	UButton* ResolutionHigh;

	UPROPERTY(meta = (BindWidget))
	UButton* ResolutionMiddle;

	UPROPERTY(meta = (BindWidget))
	UButton* ResolutionLow;

	UPROPERTY(meta = (BindWidget))
	UButton* FullScreenMode;

	UPROPERTY(meta = (BindWidget))
	UButton * WindowMode;

	UPROPERTY(meta = (BindWidget))
	UButton* ContinueButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
};
