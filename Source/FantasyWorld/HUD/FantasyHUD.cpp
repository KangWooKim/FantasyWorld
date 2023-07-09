// Fill out your copyright notice in the Description page of Project Settings.


#include "FantasyHUD.h"
#include "FantasyOverlay.h"

void AFantasyHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();

	if (World) {
		APlayerController* Controller = World->GetFirstPlayerController();

		if (Controller && FantasyOverlayClass) {

			// FantasyOverlayClass���� FantasyOverlay ������ �����մϴ�.
			FantasyOverlay = CreateWidget<UFantasyOverlay>(Controller, FantasyOverlayClass);

			// ������ ������ ����Ʈ�� �߰��մϴ�.
			FantasyOverlay->AddToViewport();
		}
	}
}
