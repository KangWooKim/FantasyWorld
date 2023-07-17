// Fill out your copyright notice in the Description page of Project Settings.


#include "TestMapHUD.h"
#include "DieMenuOverlay.h"

void ATestMapHUD::BeginPlay()
{
	Super::BeginPlay();
	/*
	UWorld* World = GetWorld();
	if (World) {
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && DieMenuOverlayClass) {
			DieMenuOverlay = CreateWidget<UDieMenuOverlay>(Controller, DieMenuOverlayClass);
			DieMenuOverlay->AddToViewport();
		}
	}
*/
}
