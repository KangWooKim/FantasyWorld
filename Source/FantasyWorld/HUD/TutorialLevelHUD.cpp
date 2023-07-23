// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelHUD.h"
#include "TutorialLevelOverlay.h"

void ATutorialLevelHUD::BeginPlay()
{
	Super::BeginPlay();
	World = World == nullptr ? GetWorld() : World;

	if (World) {
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
	}

	if (PlayerController && TutorialLevelOverlayClass) {
		TutorialLevelOverlay = CreateWidget<UTutorialLevelOverlay>(PlayerController, TutorialLevelOverlayClass);
		TutorialLevelOverlay->AddToViewport();
	}
}
