// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelHUD.h"
#include "LobbyLevelMenuOverlay.h"
#include "SelectMapOverlay.h"
#include "SelectCharacterOverlay.h"
#include "SettingsOverlay.h"

void ALobbyLevelHUD::BeginPlay()
{
	Super::BeginPlay();
	World = World == nullptr ? GetWorld() : World;
	
	if (World) {
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
		if (PlayerController && LobbyLevelMenuOverlayClass) {
			LobbyLevelMenuOverlay = CreateWidget<ULobbyLevelMenuOverlay>(PlayerController, LobbyLevelMenuOverlayClass);
			LobbyLevelMenuOverlay->AddToViewport();
		}
	}

	CreateSelectMapOverlay();
	HideSelectMapOverlay();

	CreateSelectCharacterOverlay();
	HideSelectCharacterOverlay();

	CreateSettingsOverlay();
	HideSettingsOverlay();

}

void ALobbyLevelHUD::CreateSelectMapOverlay()
{
	if (World && PlayerController && SelectMapOverlayClass) {
		SelectMapOverlay = CreateWidget<USelectMapOverlay>(PlayerController, SelectMapOverlayClass);
		SelectMapOverlay->AddToViewport();
	}
}

void ALobbyLevelHUD::ShowSelectMapOverlay()
{
	if (SelectMapOverlay) {
		SelectMapOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ALobbyLevelHUD::HideSelectMapOverlay()
{
	if (SelectMapOverlay) {
		SelectMapOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ALobbyLevelHUD::CreateSelectCharacterOverlay()
{
	if (World && PlayerController && SelectCharacterOverlayClass) {
		SelectCharacterOverlay = CreateWidget<USelectCharacterOverlay>(PlayerController, SelectCharacterOverlayClass);
		SelectCharacterOverlay->AddToViewport();
	}
}

void ALobbyLevelHUD::ShowSelectCharacterOverlay()
{
	if (SelectCharacterOverlay) {
		SelectCharacterOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

void ALobbyLevelHUD::HideSelectCharacterOverlay()
{
	if (SelectCharacterOverlay) {
		SelectCharacterOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

void ALobbyLevelHUD::CreateSettingsOverlay()
{
	if (World && PlayerController && SettingsOverlayClass) {
		SettingsOverlay = CreateWidget<USettingsOverlay>(PlayerController, SettingsOverlayClass);
		SettingsOverlay->AddToViewport();
	}
}

void ALobbyLevelHUD::ShowSettingsOverlay()
{
	if (SettingsOverlay) {
		SettingsOverlay->SetVisibility(ESlateVisibility::Visible);
	}
	if (SettingsOverlay) {
		SettingsOverlay->HideContinueButton();
	}
}

void ALobbyLevelHUD::HideSettingsOverlay()
{
	if (SettingsOverlay) {
		SettingsOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
