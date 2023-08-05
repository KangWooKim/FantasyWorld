// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "LobbyLevelHUD.h"
#include "Engine/Texture2D.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"

void USelectCharacterOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	if (CountessButton) {
		CountessButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnCountessButtonClicked);
		CountessButton->OnHovered.AddDynamic(this, &USelectCharacterOverlay::OnHoveredCountessButton);
		CountessButton->OnUnhovered.AddDynamic(this, &USelectCharacterOverlay::OnUnHoveredCountessButton);
	}

	if (PhaseButton) {
		PhaseButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnPhaseButtonClicked);
		PhaseButton->OnHovered.AddDynamic(this, &USelectCharacterOverlay::OnHoverPhaseButton);
		PhaseButton->OnUnhovered.AddDynamic(this, &USelectCharacterOverlay::OnUnHoverPhaseButton);
	}

	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnQuitButtonClicked);
	}

}

void USelectCharacterOverlay::OnCountessButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance) {
		GameInstance->SetSelectedCharacter(ECharacterTypes::Countess);
		GameInstance->StartLevel();
	}
}

void USelectCharacterOverlay::OnPhaseButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	if (GameInstance) {
		GameInstance->SetSelectedCharacter(ECharacterTypes::Phase);
		GameInstance->StartLevel();
	}
}

void USelectCharacterOverlay::OnQuitButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	this->SetVisibility(ESlateVisibility::Hidden);

	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSelectMapOverlay();
}

void USelectCharacterOverlay::OnHoveredCountessButton()
{
	if (MediaSources.Num() > 1) {
		MediaPlayer->OpenSource(MediaSources[0]);
		MediaPlayer->Play();
	}


	MediaTexture->SetMediaPlayer(MediaPlayer);

	// Set the brush to the MediaTexture.
	Brush.SetResourceObject(MediaTexture);
	Image->SetBrush(Brush);
}

void USelectCharacterOverlay::OnUnHoveredCountessButton()
{
	FSlateBrush EmptyBrush;
	EmptyBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	Image->SetBrush(EmptyBrush);
}

void USelectCharacterOverlay::OnHoverPhaseButton()
{
	if (MediaSources.Num() > 1) {
		MediaPlayer->OpenSource(MediaSources[1]);
		MediaPlayer->Play();
	}


	MediaTexture->SetMediaPlayer(MediaPlayer);

	// Set the brush to the MediaTexture.
	Brush.SetResourceObject(MediaTexture);
	Image->SetBrush(Brush);
}

void USelectCharacterOverlay::OnUnHoverPhaseButton()
{
	FSlateBrush EmptyBrush;
	EmptyBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	Image->SetBrush(EmptyBrush);
}
