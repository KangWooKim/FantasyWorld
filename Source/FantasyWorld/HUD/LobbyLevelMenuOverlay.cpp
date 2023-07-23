// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ULobbyLevelMenuOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked);
	}

	if (TutorialButton) {
		TutorialButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnTutorialButtonClicked);
	}

	if (SettingsButton) {
		SettingsButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnSettingsButtonClicked);
	}

	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnQuitButtonClicked);
	}
}

void ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked() {

}

void ULobbyLevelMenuOverlay::OnTutorialButtonClicked()
{
	UGameplayStatics::OpenLevel(this, "/Content/Maps/TutorialLevel");
}

void ULobbyLevelMenuOverlay::OnSettingsButtonClicked()
{

}

void ULobbyLevelMenuOverlay::OnQuitButtonClicked()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
