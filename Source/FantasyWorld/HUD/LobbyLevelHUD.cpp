// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelHUD.h"
#include "LobbyLevelMenuOverlay.h"

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
}
