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

			// FantasyOverlayClass에서 FantasyOverlay 위젯을 생성합니다.
			FantasyOverlay = CreateWidget<UFantasyOverlay>(Controller, FantasyOverlayClass);

			// 생성된 위젯을 뷰포트에 추가합니다.
			FantasyOverlay->AddToViewport();
		}
	}
}
