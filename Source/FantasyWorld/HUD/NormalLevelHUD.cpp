// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalLevelHUD.h"
#include "MissionClearOverlay.h"
#include "DieMenuOverlay.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/HUD/MissionPreviewOverlay.h"
#include "CombatOverlay.h"
#include "SettingsOverlay.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Slider.h"


ANormalLevelHUD::ANormalLevelHUD()
{
}

void ANormalLevelHUD::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	if (World && PlayerController && MissionPreviewOverlayClass) {
		MissionPreviewOverlay = CreateWidget<UMissionPreviewOverlay>(PlayerController, MissionPreviewOverlayClass);
		MissionPreviewOverlay->AddToViewport();
		MissionPreviewOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANormalLevelHUD::RemoveMissionPreviewOverlay, 5.f, false);

	CreateCombatOverlay();
	ShowCombatOverlay();

	CreateSettingsOverlay();
	HideSettingsOverlay();

	CreateMissionClearOverlay();
	HideMissionClearOverlay();

	CreateDieMenuOverlay();
	HideDieMenuOverlay();

	
}

void ANormalLevelHUD::RemoveMissionPreviewOverlay() 
{
	if (MissionPreviewOverlay) {
		MissionPreviewOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ANormalLevelHUD::CreateCombatOverlay()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	if (World && PlayerController && CombatOverlayClass) {
		CombatOverlay = CreateWidget<UCombatOverlay>(PlayerController, CombatOverlayClass);
		CombatOverlay->AddToViewport();
	}
}

void ANormalLevelHUD::ShowCombatOverlay()
{
	if (CombatOverlay) {
		CombatOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ANormalLevelHUD::HideCombatOverlay()
{
	if (CombatOverlay) {
		CombatOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ANormalLevelHUD::CreateMissionClearOverlay()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	if (World && PlayerController && MissionClearOverlayClass) {
		MissionClearOverlay = CreateWidget<UMissionClearOverlay>(PlayerController, MissionClearOverlayClass);
		MissionClearOverlay->AddToViewport();
	}
}

void ANormalLevelHUD::ShowMissionClearOverlay(int32 SlayedEnemy)
{
	if (MissionClearOverlay) {
		MissionClearOverlay->SetVisibility(ESlateVisibility::Visible);
		MissionClearOverlay->UpdateTextBlock(SlayedEnemy);
	}
}

void ANormalLevelHUD::HideMissionClearOverlay()
{
	if (MissionClearOverlay) {
		MissionClearOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ANormalLevelHUD::CreateDieMenuOverlay()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	if (World && PlayerController && DieMenuOverlayClass) {
		DieMenuOverlay = CreateWidget<UDieMenuOverlay>(PlayerController, DieMenuOverlayClass);
		DieMenuOverlay->AddToViewport();
	}
}

void ANormalLevelHUD::ShowDieMenuOverlay(int32 SlayedEnemy, int32 LeftEnemy)
{
	if (DieMenuOverlay) {
		DieMenuOverlay->SetVisibility(ESlateVisibility::Visible);
		DieMenuOverlay->UpdateTextBlock(SlayedEnemy, LeftEnemy);
	}
}

void ANormalLevelHUD::HideDieMenuOverlay()
{
	if (DieMenuOverlay) {
		DieMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ANormalLevelHUD::CreateSettingsOverlay()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	if (World && PlayerController && SettingsOverlayClass) {
		SettingsOverlay = CreateWidget<USettingsOverlay>(PlayerController, SettingsOverlayClass);
		SettingsOverlay->AddToViewport();
	}
}

void ANormalLevelHUD::ShowSettingsOverlay()
{
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	if (SettingsOverlay) {
		SettingsOverlay->SetVisibility(ESlateVisibility::Visible);
		if (GameInstance) {
			SettingsOverlay->BGMVolume->SetValue(GameInstance->GetBGMVolume());
			SettingsOverlay->EffectSoundVolume->SetValue(GameInstance->GetEffectVolume());
			SettingsOverlay->MouseSensitivitySlider->SetValue(GameInstance->GetMouseSensitivity() / 5);
		}
	}
}

void ANormalLevelHUD::HideSettingsOverlay()
{
	if (SettingsOverlay) {
		SettingsOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}