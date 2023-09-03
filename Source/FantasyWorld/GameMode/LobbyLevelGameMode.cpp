// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"

// �⺻ ������. LobbyLevelGameMode Ŭ������ �ν��Ͻ��� ������ �� ȣ��˴ϴ�.
ALobbyLevelGameMode::ALobbyLevelGameMode() {
    // �� ���Ͱ� Tick �Լ��� ȣ���� �� �ִ��� ���θ� �����մϴ�. ���⼭�� true�� �����Ǿ� �ֽ��ϴ�.
    PrimaryActorTick.bCanEverTick = true;
}

// BeginPlay �Լ�. ������ ���۵Ǹ� ȣ��˴ϴ�.
void ALobbyLevelGameMode::BeginPlay()
{
    // �θ� Ŭ������ BeginPlay�� ȣ���Ͽ� �⺻ ������ �����մϴ�.
    Super::BeginPlay();

    // GetWorld �Լ��� ����Ͽ� ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
    PlayerController = GetWorld()->GetFirstPlayerController();

    // ���� ���� �ν��Ͻ��� �����ɴϴ�.
    GameInstance = Cast<UMyGameInstance>(GetGameInstance());

    // �÷��̾� ��Ʈ�ѷ��� ��ȿ�ϸ� ���콺 Ŀ���� ���̰� �����մϴ�.
    if (PlayerController) {
        PlayerController->SetShowMouseCursor(true);
    }

    // ���ο� ����� ������Ʈ ��ü�� �����մϴ�.
    BGMComponent = NewObject<UAudioComponent>(this);

    // BGMComponent�� ��ȿ�ϸ� �Ʒ� ������ �����մϴ�.
    if (BGMComponent)
    {
        // ��������� �����մϴ�.
        BGMComponent->SetSound(BackGroundMusic);

        // �ڵ����� Ȱ��ȭ���� �ʵ��� �����մϴ�.
        BGMComponent->bAutoActivate = false;

        // ������Ʈ�� ����մϴ�.
        BGMComponent->RegisterComponent();

        // ��Ʈ ������Ʈ�� �����մϴ�.
        BGMComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

        // ��������� ����մϴ�.
        BGMComponent->Play();
    }
}

// Tick �Լ�. �����Ӹ��� ȣ��˴ϴ�.
void ALobbyLevelGameMode::Tick(float DeltaTime)
{
    // ��������� ������ ���� �ν��Ͻ����� ������ ������ �����մϴ�.
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
}
