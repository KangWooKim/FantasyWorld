// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectMapOverlay.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "LobbyLevelHUD.h"

// USelectMapOverlay의 초기 설정을 하는 메서드
void USelectMapOverlay::NativeConstruct()
{
    Super::NativeConstruct(); // 부모 클래스의 NativeConstruct 메서드 호출

    // 현재 세계에서 플레이어 컨트롤러의 HUD를 가져와 HUD 변수에 할당
    HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

    // GrassLandButton이 존재하면, 클릭 시 OnGrassLandButtonClicked 함수를 호출하도록 설정
    if (GrassLandButton) {
        GrassLandButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnGrassLandButtonClicked);
    }

    // IceLandButton이 존재하면, 클릭 시 OnIceLandButtonClicked 함수를 호출하도록 설정
    if (IceLandButton) {
        IceLandButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnIceLandButtonClicked);
    }

    // InfinityModeButton이 존재하면, 클릭 시 OnInfinityModeButtonClicked 함수를 호출하도록 설정
    if (InfinityModeButton) {
        InfinityModeButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnInfinityModeButtonClicked);
    }

    // QuitButton이 존재하면, 클릭 시 OnQuitButtonClicked 함수를 호출하도록 설정
    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &USelectMapOverlay::OnQuitButtonClicked);
    }
}

// GrassLandButton을 클릭했을 때 호출되는 메서드
void USelectMapOverlay::OnGrassLandButtonClicked()
{
    // ButtonClickedSound가 존재하면 사운드를 재생
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 선택한 맵의 이름을 설정
    MapName = TEXT("/Game/Maps/GrassLand");

    // 게임 인스턴스를 가져와서 맵 이름을 설정
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    // 캐릭터 선택 오버레이를 표시하고, 맵 선택 오버레이를 숨김
    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

// IceLandButton을 클릭했을 때 호출되는 메서드
void USelectMapOverlay::OnIceLandButtonClicked()
{
    // ButtonClickedSound가 존재하면 사운드를 재생
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 선택한 맵의 이름을 설정
    MapName = TEXT("/Game/Maps/IceLand");

    // 게임 인스턴스를 가져와서 맵 이름을 설정
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    // 캐릭터 선택 오버레이를 표시하고, 맵 선택 오버레이를 숨김
    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}


// 무한모드 버튼 클릭 이벤트 처리 함수
void USelectMapOverlay::OnInfinityModeButtonClicked()
{
    // 버튼 클릭 사운드가 설정되어 있다면, 사운드를 재생한다.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 0에서 2까지의 랜덤한 정수를 생성한다.
    const int RandomNumber = FMath::RandRange(0, 2);

    // 랜덤한 정수에 따라 다른 무한모드 맵을 선택한다.
    if (RandomNumber == 0) {
        MapName = TEXT("/Game/Maps/InfinityLand1");
    }
    else if (RandomNumber == 1) {
        MapName = TEXT("/Game/Maps/InfinityLand2");
    }
    else if (RandomNumber == 2) {
        MapName = TEXT("/Game/Maps/InfinityLand");
    }

    // 게임 인스턴스를 가져와서 선택한 맵 이름을 설정한다.
    UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
    if (GameInstance) {
        GameInstance->SetMapName(MapName);
    }

    // 캐릭터 선택 오버레이를 보여주고, 맵 선택 오버레이를 숨긴다.
    HUD->ShowSelectCharacterOverlay();
    HUD->HideSelectMapOverlay();
}

// 종료 버튼 클릭 이벤트 처리 함수
void USelectMapOverlay::OnQuitButtonClicked()
{
    // 버튼 클릭 사운드가 설정되어 있다면, 사운드를 재생한다.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 이 오버레이를 숨긴다.
    this->SetVisibility(ESlateVisibility::Hidden);
}
