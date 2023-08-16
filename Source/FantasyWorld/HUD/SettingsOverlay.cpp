// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsOverlay.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Slider.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "LobbyLevelHUD.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "GameFramework/GameUserSettings.h"



void USettingsOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	Controller = Controller == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : Controller;

	if (BGMVolume) {
		BGMVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		if (GameInstance) {
			BGMVolume->SetValue(GameInstance->GetBGMVolume());
		}
		else {
			BGMVolume->SetValue(1.f);
		}
		
		BGMVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleBGMVolumeChanged);
	}

	if (EffectSoundVolume) {
		EffectSoundVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		if (GameInstance) {
			EffectSoundVolume->SetValue(GameInstance->GetEffectVolume());
		}
		else {
			EffectSoundVolume->SetValue(1.f);
		}
		
		EffectSoundVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleEffectSoundVolumeChanged);
	}

	if (MouseSensitivitySlider) {
		MouseSensitivitySlider->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		if (GameInstance) {
			MouseSensitivitySlider->SetValue(GameInstance->GetMouseSensitivity() / 5);
		}
		else {
			MouseSensitivitySlider->SetValue(1.f);
		}
		MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleMouseSensitivitySliderChanged);
	}

	if (ResolutionHigh) {
		ResolutionHigh->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionHighButtonClicked);
	}

	if (ResolutionMiddle) {
		ResolutionMiddle->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionMiddleButtonClicked);
	}

	if (ResolutionLow) {
		ResolutionLow->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionLowButtonClicked);
	}

	if (FullScreenMode) {
		FullScreenMode->OnClicked.AddDynamic(this, &USettingsOverlay::OnFullScreenModeButtonClicked);
	}

	if (WindowMode) {
		WindowMode->OnClicked.AddDynamic(this, &USettingsOverlay::OnWindowModeButtonClicked);
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
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	if (GameInstance) {
		GameInstance->SetBGMVolume(value);
	}
}

void USettingsOverlay::HandleEffectSoundVolumeChanged(float value)
{
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	if (GameInstance) {
		GameInstance->SetEffectVolume(value);
	}
}

void USettingsOverlay::HandleMouseSensitivitySliderChanged(float value)
{
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	if (GameInstance) {
		GameInstance->SetMouseSensitivity(value * 5);
	}
}

void USettingsOverlay::OnResolutionHighButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(3840, 2160));
		Settings->ApplySettings(true);
	}
}

void USettingsOverlay::OnResolutionMiddleButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		Settings->ApplySettings(true);
	}
}

void USettingsOverlay::OnResolutionLowButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(1280, 720));
		Settings->ApplySettings(true);
	}
}

void USettingsOverlay::OnFullScreenModeButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings) {
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		Settings->ApplySettings(true);
	}

}

void USettingsOverlay::OnWindowModeButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;
	if (Settings) {
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		Settings->ApplySettings(true);
	}

}

void USettingsOverlay::OnQuitButtonClicked()
{
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	if (HUD) {
		HUD->HideSettingsOverlay();
	}
	else {
		UWorld* World = GetWorld(); // Get the UWorld pointer.
		if (World != nullptr) // Always check if the pointer is valid.
		{
			FString mapName = TEXT("/Game/Maps/LobbyLevel"); // Change this to your map's name.
			UGameplayStatics::OpenLevel(World, FName(*mapName)); // Open the specified level.
		}
	}
	
}

void USettingsOverlay::OnContinueButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->SetPause(false);
		}
		Controller = Controller == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : Controller;
		if (Controller) {
			Controller->RePossess();
			Controller->SetShowMouseCursor(false);
		}
	}
	this->SetVisibility(ESlateVisibility::Hidden);
}

void USettingsOverlay::HideContinueButton()
{
	ContinueButton->SetVisibility(ESlateVisibility::Hidden);
}

