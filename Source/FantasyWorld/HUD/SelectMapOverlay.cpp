// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMapOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "LobbyLevelHUD.h"

void USelectMapOverlay::NativeConstruct()
{
    Super::NativeConstruct();

    HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    if (GrassLandButton) {
        GrassLandButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnGrassLandButtonClicked);
    }

    if (IceLandButton) {
        IceLandButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnIceLandButtonClicked);
    }

    if (InfinityModeButton) {
        InfinityModeButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnInfinityModeButtonClicked);
    }

    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnQuitButtonClicked);
    }

}

void USelectMapOverlay::OnGrassLandButtonClicked()
{
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }
    MapName = TEXT("/Game/Maps/GrassLand");
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

void USelectMapOverlay::OnIceLandButtonClicked()
{
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }
    MapName = TEXT("/Game/Maps/IceLand");
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

void USelectMapOverlay::OnInfinityModeButtonClicked()
{
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    const int RandomNumber = FMath::RandRange(0, 2);
    if (RandomNumber == 0) {
        MapName = TEXT("/Game/Maps/InfinityLand1");
    }
    else if (RandomNumber == 1) {
        MapName = TEXT("/Game/Maps/InfinityLand2");
    }
    else if (RandomNumber == 2) {
        MapName = TEXT("/Game/Maps/InfinityLand");
    }
    
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

void USelectMapOverlay::OnQuitButtonClicked()
{
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }
    this->SetVisibility(ESlateVisibility::Hidden);
}
