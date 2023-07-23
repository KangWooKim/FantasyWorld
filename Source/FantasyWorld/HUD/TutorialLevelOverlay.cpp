// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelOverlay.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"


void UTutorialLevelOverlay::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentVideoIndex = 0;

    if (OkButton) {
        OkButton->OnClicked.AddDynamic(this, &UTutorialLevelOverlay::OnOkButtonClicked);
    }

    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &UTutorialLevelOverlay::OnQuitButtonClicked);
    }
    

    // Assuming MediaSources array is already populated.
    // If not, you will need to populate it with UMediaSource objects.
    if (MediaSources.Num() > 0) {
        MediaPlayer->OpenSource(MediaSources[0]);
        MediaPlayer->Play();
    }

    
    MediaTexture->SetMediaPlayer(MediaPlayer);

    // Set the brush to the MediaTexture.
    Brush.SetResourceObject(MediaTexture);
    Image->SetBrush(Brush);

	
}

void UTutorialLevelOverlay::OnOkButtonClicked()
{
    this->SetVisibility(ESlateVisibility::Hidden);
}

void UTutorialLevelOverlay::OnQuitButtonClicked()
{
    UWorld* World = GetWorld(); // Get the UWorld pointer.
    if (World != nullptr) // Always check if the pointer is valid.
    {
        FString mapName = TEXT("/Game/Maps/LobbyLevel"); // Change this to your map's name.
        UGameplayStatics::OpenLevel(World, FName(*mapName)); // Open the specified level.
    }
}

void UTutorialLevelOverlay::PlayNextVideo()
{
    this->SetVisibility(ESlateVisibility::Visible);
    ++CurrentVideoIndex;
    if (MediaSources.IsValidIndex(CurrentVideoIndex)) {
        MediaPlayer->OpenSource(MediaSources[CurrentVideoIndex]);
        MediaPlayer->Play();
    }
    if (MediaTexture) {
        MediaTexture->SetMediaPlayer(MediaPlayer);
    }
    
     Brush.SetResourceObject(MediaTexture);
     Image->SetBrush(Brush);
    
}