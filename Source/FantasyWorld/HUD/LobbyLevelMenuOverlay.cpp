// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyLevelHUD.h"
#include "Engine/Texture2D.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"

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

	if (MediaSources.Num() > 0) {
		MediaPlayer->OpenSource(MediaSources[0]);
		MediaPlayer->Play();
	}


	MediaTexture->SetMediaPlayer(MediaPlayer);

	// Set the brush to the MediaTexture.
	Brush.SetResourceObject(MediaTexture);
	Image->SetBrush(Brush);
}

void ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked() {
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSelectMapOverlay();
}

void ULobbyLevelMenuOverlay::OnTutorialButtonClicked(){

	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
	UWorld* World = GetWorld(); // Get the UWorld pointer.
	if (World != nullptr) // Always check if the pointer is valid.
	{
		FString mapName = TEXT("/Game/Maps/TutorialLevel"); // Change this to your map's name.
		UGameplayStatics::OpenLevel(World, FName(*mapName)); // Open the specified level.
	}
}

void ULobbyLevelMenuOverlay::OnSettingsButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSettingsOverlay();
}

void ULobbyLevelMenuOverlay::OnQuitButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
