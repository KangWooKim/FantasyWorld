// Fill out your copyright notice in the Description page of Project Settings.


#include "DieMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UDieMenuOverlay::NativeConstruct()
{
	Super::NativeConstruct();


	if (ReturnToMenu) {
		ReturnToMenu->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnReturnToMenuButtonClicked);
	}

	if (RestartLevel) {
		RestartLevel->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnRestartLevelButtonClicked);
	}
}

void UDieMenuOverlay::OnReturnToMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(this, "/Game/Maps/LobbyMenu");
}

void UDieMenuOverlay::OnRestartLevelButtonClicked()
{
	UGameplayStatics::OpenLevel(this, "/Game/Maps/LobbyMenu");
}