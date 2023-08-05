// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionClearOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void UMissionClearOverlay:: NativeConstruct()
{
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &UMissionClearOverlay::OnQuitButtonClicked);
	}
}

void UMissionClearOverlay::OnQuitButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UWorld* World = GetWorld(); // Get the UWorld pointer.
	if (World != nullptr) // Always check if the pointer is valid.
	{
		FString mapName = TEXT("/Game/Maps/LobbyLevel"); // Change this to your map's name.
		UGameplayStatics::OpenLevel(World, FName(*mapName)); // Open the specified level.
	}
}

void UMissionClearOverlay::UpdateTextBlock(int32 SlayedEnemy)
{
	FString CombinedString = FString::Printf(TEXT("You saved the world by cutting down %d enemies"), SlayedEnemy);
	FText NewText = FText::FromString(CombinedString);
	Text->SetText(NewText);
}