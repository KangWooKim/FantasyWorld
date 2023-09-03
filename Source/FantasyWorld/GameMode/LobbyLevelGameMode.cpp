// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"

// 기본 생성자. LobbyLevelGameMode 클래스의 인스턴스가 생성될 때 호출됩니다.
ALobbyLevelGameMode::ALobbyLevelGameMode() {
    // 이 액터가 Tick 함수를 호출할 수 있는지 여부를 설정합니다. 여기서는 true로 설정되어 있습니다.
    PrimaryActorTick.bCanEverTick = true;
}

// BeginPlay 함수. 게임이 시작되면 호출됩니다.
void ALobbyLevelGameMode::BeginPlay()
{
    // 부모 클래스의 BeginPlay를 호출하여 기본 로직을 실행합니다.
    Super::BeginPlay();

    // GetWorld 함수를 사용하여 첫 번째 플레이어 컨트롤러를 가져옵니다.
    PlayerController = GetWorld()->GetFirstPlayerController();

    // 현재 게임 인스턴스를 가져옵니다.
    GameInstance = Cast<UMyGameInstance>(GetGameInstance());

    // 플레이어 컨트롤러가 유효하면 마우스 커서를 보이게 설정합니다.
    if (PlayerController) {
        PlayerController->SetShowMouseCursor(true);
    }

    // 새로운 오디오 컴포넌트 객체를 생성합니다.
    BGMComponent = NewObject<UAudioComponent>(this);

    // BGMComponent가 유효하면 아래 로직을 실행합니다.
    if (BGMComponent)
    {
        // 배경음악을 설정합니다.
        BGMComponent->SetSound(BackGroundMusic);

        // 자동으로 활성화되지 않도록 설정합니다.
        BGMComponent->bAutoActivate = false;

        // 컴포넌트를 등록합니다.
        BGMComponent->RegisterComponent();

        // 루트 컴포넌트에 연결합니다.
        BGMComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

        // 배경음악을 재생합니다.
        BGMComponent->Play();
    }
}

// Tick 함수. 프레임마다 호출됩니다.
void ALobbyLevelGameMode::Tick(float DeltaTime)
{
    // 배경음악의 볼륨을 게임 인스턴스에서 가져온 값으로 설정합니다.
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
}
