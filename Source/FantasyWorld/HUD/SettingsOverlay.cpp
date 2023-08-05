// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsOverlay.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Slider.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "LobbyLevelHUD.h"

void USettingsOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	if (BGMVolume) {
		BGMVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		BGMVolume->SetValue(1.f);
		BGMVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleBGMVolumeChanged);
	}

	if (EffectSoundVolume) {
		EffectSoundVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		EffectSoundVolume->SetValue(1.f);
		EffectSoundVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleEffectSoundVolumeChanged);
	}

	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnQuitButtonClicked);
	}

	if (ContinueButton) {
		ContinueButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnContinueButtonClicked);
	}
}

void USettingsOverlay::HandleBGMVolumeChanged(float value)
{
	GameInstance->SetBGMVolume(value);
}

void USettingsOverlay::HandleEffectSoundVolumeChanged(float value)
{
	GameInstance->SetEffectVolume(value);
}

void USettingsOverlay::OnQuitButtonClicked()
{
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->HideSettingsOverlay();
}

void USettingsOverlay::OnContinueButtonClicked()
{
	this->SetVisibility(ESlateVisibility::Hidden);
}

void USettingsOverlay::HideContinueButton()
{
	ContinueButton->SetVisibility(ESlateVisibility::Hidden);
}

