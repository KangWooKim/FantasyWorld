// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"

ALobbyLevelGameMode::ALobbyLevelGameMode() {
    PrimaryActorTick.bCanEverTick = true;
}

void ALobbyLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    PlayerController = GetWorld()->GetFirstPlayerController();
    GameInstance = Cast<UMyGameInstance>(GetGameInstance());

    if (PlayerController) {
        PlayerController->SetShowMouseCursor(true);
    }

    BGMComponent = NewObject<UAudioComponent>(this);
    if (BGMComponent)
    {
        BGMComponent->SetSound(BackGroundMusic);
        BGMComponent->bAutoActivate = false;
        BGMComponent->RegisterComponent();
        BGMComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        BGMComponent->Play();
    }
}

void ALobbyLevelGameMode::Tick(float DeltaTime)
{
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
}
