// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMapOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "LobbyLevelHUD.h"

// USelectMapOverlay�� �ʱ� ������ �ϴ� �޼���
void USelectMapOverlay::NativeConstruct()
{
    Super::NativeConstruct(); // �θ� Ŭ������ NativeConstruct �޼��� ȣ��

    // ���� ���迡�� �÷��̾� ��Ʈ�ѷ��� HUD�� ������ HUD ������ �Ҵ�
    HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    // GrassLandButton�� �����ϸ�, Ŭ�� �� OnGrassLandButtonClicked �Լ��� ȣ���ϵ��� ����
    if (GrassLandButton) {
        GrassLandButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnGrassLandButtonClicked);
    }

    // IceLandButton�� �����ϸ�, Ŭ�� �� OnIceLandButtonClicked �Լ��� ȣ���ϵ��� ����
    if (IceLandButton) {
        IceLandButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnIceLandButtonClicked);
    }

    // InfinityModeButton�� �����ϸ�, Ŭ�� �� OnInfinityModeButtonClicked �Լ��� ȣ���ϵ��� ����
    if (InfinityModeButton) {
        InfinityModeButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnInfinityModeButtonClicked);
    }

    // QuitButton�� �����ϸ�, Ŭ�� �� OnQuitButtonClicked �Լ��� ȣ���ϵ��� ����
    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnQuitButtonClicked);
    }
}

// GrassLandButton�� Ŭ������ �� ȣ��Ǵ� �޼���
void USelectMapOverlay::OnGrassLandButtonClicked()
{
    // ButtonClickedSound�� �����ϸ� ���带 ���
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // ������ ���� �̸��� ����
    MapName = TEXT("/Game/Maps/GrassLand");

    // ���� �ν��Ͻ��� �����ͼ� �� �̸��� ����
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    // ĳ���� ���� �������̸� ǥ���ϰ�, �� ���� �������̸� ����
    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

// IceLandButton�� Ŭ������ �� ȣ��Ǵ� �޼���
void USelectMapOverlay::OnIceLandButtonClicked()
{
    // ButtonClickedSound�� �����ϸ� ���带 ���
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // ������ ���� �̸��� ����
    MapName = TEXT("/Game/Maps/IceLand");

    // ���� �ν��Ͻ��� �����ͼ� �� �̸��� ����
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    // ĳ���� ���� �������̸� ǥ���ϰ�, �� ���� �������̸� ����
    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}


// ���Ѹ�� ��ư Ŭ�� �̺�Ʈ ó�� �Լ�
void USelectMapOverlay::OnInfinityModeButtonClicked()
{
    // ��ư Ŭ�� ���尡 �����Ǿ� �ִٸ�, ���带 ����Ѵ�.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 0���� 2������ ������ ������ �����Ѵ�.
    const int RandomNumber = FMath::RandRange(0, 2);

    // ������ ������ ���� �ٸ� ���Ѹ�� ���� �����Ѵ�.
    if (RandomNumber == 0) {
        MapName = TEXT("/Game/Maps/InfinityLand1");
    }
    else if (RandomNumber == 1) {
        MapName = TEXT("/Game/Maps/InfinityLand2");
    }
    else if (RandomNumber == 2) {
        MapName = TEXT("/Game/Maps/InfinityLand");
    }

    // ���� �ν��Ͻ��� �����ͼ� ������ �� �̸��� �����Ѵ�.
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    // ĳ���� ���� �������̸� �����ְ�, �� ���� �������̸� �����.
    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

// ���� ��ư Ŭ�� �̺�Ʈ ó�� �Լ�
void USelectMapOverlay::OnQuitButtonClicked()
{
    // ��ư Ŭ�� ���尡 �����Ǿ� �ִٸ�, ���带 ����Ѵ�.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // �� �������̸� �����.
    this->SetVisibility(ESlateVisibility::Hidden);
}
