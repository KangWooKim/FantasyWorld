// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalLevelGameMode.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/Character/BaseCharacter.h"
#include "FantasyWorld/Character/Countess.h"
#include "FantasyWorld/Character/Phase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Components/AudioComponent.h"
#include "FantasyWorld/Character/Enemy/Enemy.h"
#include "FantasyWorld/HUD/NormalLevelHUD.h"
#include "FantasyWorld/HUD/CombatOverlay.h"

// �⺻ ���� ��� Ŭ������ ��ӹ޴� ANormalLevelGameMode�� �������Դϴ�.
ANormalLevelGameMode::ANormalLevelGameMode()
{
}

// ������ �ʱ� ������ ó���ϴ� BeginPlay �޼����Դϴ�.
void ANormalLevelGameMode::BeginPlay()
{
    // �θ� Ŭ������ BeginPlay �޼��带 ȣ���մϴ�.
    Super::BeginPlay();

    // ���� ���带 �����ͼ� ������ �����մϴ�.
    World = GetWorld();

    // GameInstance�� null�� ��� ���� ���� �ν��Ͻ��� �����ͼ� ������ �����մϴ�.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // PlayerController�� null�� ��� ���� �÷��̾� ��Ʈ�ѷ��� �����ͼ� ������ �����մϴ�.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

    // PlayerController�� null�� �ƴ� ���, HUD�� �����մϴ�.
    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
    }

    // ��� ������ ���� ����� ������Ʈ�� �����ϰ� �����մϴ�.
    BGMComponent = NewObject<UAudioComponent>(this);
    if (BGMComponent)
    {
        BGMComponent->SetSound(BackGroundMusic);
        BGMComponent->bAutoActivate = false;
        BGMComponent->RegisterComponent();
        BGMComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        BGMComponent->Play();
        BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
    }

    // �÷��̾� ĳ���͸� �����մϴ�.
    SpawnPlayerCharacter();

    // ���忡 �����ϴ� ���� ��� �����ͼ� EnemyList�� �����մϴ�.
    TArray<AActor*> EnemyList;
    UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), EnemyList);

    // ���� ���� �����մϴ�.
    LeftEnemyNumber = EnemyList.Num();
    TotalEnemyNumber = EnemyList.Num();

    // �� ���� EnemyDeath �̺�Ʈ�� ���� �ڵ鷯�� �����մϴ�.
    for (AActor* Enemy : EnemyList) {
        ABaseEnemy* E = Cast<ABaseEnemy>(Enemy);
        E->EnemyDeath.AddDynamic(this, &ANormalLevelGameMode::EnemyDie);
    }
}

// �����Ӹ��� ȣ��Ǵ� Tick �޼����Դϴ�.
void ANormalLevelGameMode::Tick(float DeltaTime)
{
    // �θ� Ŭ������ Tick �޼��带 ȣ���մϴ�.
    Super::Tick(DeltaTime);

    // GameInstance�� null�� ��� ���� ���� �ν��Ͻ��� �����ͼ� ������ �����մϴ�.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // ��� ������ ������ ������Ʈ�մϴ�.
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());

    // PlayerController�� null�� ��� ���� �÷��̾� ��Ʈ�ѷ��� �����ͼ� ������ �����մϴ�.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

    // PlayerController�� HUD�� null�� �ƴ� ���, HUD�� �� ���� �ؽ�Ʈ�� ������Ʈ�մϴ�.
    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
        if (HUD) {
            HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, TotalEnemyNumber));
        }
    }
}


// �÷��̾ �׾��� �� ȣ��Ǵ� �޼����Դϴ�.
void ANormalLevelGameMode::PlayerDie()
{
    // PlayerController�� null�� ��� ���� �÷��̾� ��Ʈ�ѷ��� �����ͼ� ������ �����մϴ�.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

    // HUD�� null�� ��� ���� HUD�� �����ͼ� ������ �����մϴ�.
    HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

    // ����, PlayerController, HUD�� null�� �ƴ϶�� �Ʒ� ������ �����մϴ�.
    if (World && PlayerController && HUD) {
        PlayerController->UnPossess(); // �÷��̾��� ĳ���� ������ �����մϴ�.
        PlayerController->SetShowMouseCursor(true); // ���콺 Ŀ���� ǥ���մϴ�.
        HUD->ShowDieMenuOverlay(TotalEnemyNumber - LeftEnemyNumber, LeftEnemyNumber); // ���� �޴� �������̸� ǥ���մϴ�.
    }
}

// ���� �׾��� �� ȣ��Ǵ� �޼����Դϴ�.
void ANormalLevelGameMode::EnemyDie()
{
    LeftEnemyNumber = LeftEnemyNumber - 1; // ���� ���� ���� ���ҽ�ŵ�ϴ�.

    // ��� ���� �׾��� ��� StageClear �޼��带 ȣ���մϴ�.
    if (LeftEnemyNumber == 0) {
        StageClear();
    }
    // ���� ���� ���� HUD�� ������Ʈ�մϴ�.
    HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, TotalEnemyNumber));
}

// �÷��̾� ĳ���͸� �����ϴ� �޼����Դϴ�.
void ANormalLevelGameMode::SpawnPlayerCharacter()
{
    // GameInstance�� null�� ��� ���� ���� �ν��Ͻ��� �����ͼ� ������ �����մϴ�.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // ����� GameInstance�� null�� �ƴϸ� �Ʒ� ������ �����մϴ�.
    if (GetWorld() && GameInstance) {
        FActorSpawnParameters SpawnParams;
        ECharacterTypes CharacterTypes = GameInstance->GetSelectedCharacter(); // ���õ� ĳ������ Ÿ���� �����ɴϴ�.

        // ���õ� ĳ���Ϳ� ���� ĳ���͸� �����մϴ�.
        if (CountessClass && PhaseClass) {
            TArray<AActor*> PlayerStarts;
            UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

            if (CharacterTypes == ECharacterTypes::Countess) {
                Character = World->SpawnActor<ACountess>(CountessClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
            }
            else if (CharacterTypes == ECharacterTypes::Phase) {
                Character = World->SpawnActor<APhase>(PhaseClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
            }
        }
    }

    // ĳ���Ͱ� ���������� �����Ǿ����� �÷��̾� ��Ʈ�ѷ��� �� ĳ���͸� �����մϴ�.
    if (Character) {
        PlayerController->Possess(Character);
        Character->OnDeathEnd.AddDynamic(this, &ANormalLevelGameMode::PlayerDie); // ĳ������ OnDeathEnd �̺�Ʈ�� ���� �ڵ鷯�� �����մϴ�.
    }
}

// ��� ���� óġ�Ͽ� ���������� Ŭ�������� �� ȣ��Ǵ� �޼����Դϴ�.
void ANormalLevelGameMode::StageClear()
{
    // HUD�� null�� ��� ���� HUD�� �����ͼ� ������ �����մϴ�.
    HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

    // ����, PlayerController, HUD�� null�� �ƴ϶�� �Ʒ� ������ �����մϴ�.
    if (World && PlayerController && HUD) {
        PlayerController->UnPossess(); // �÷��̾��� ĳ���� ������ �����մϴ�.
        PlayerController->SetShowMouseCursor(true); // ���콺 Ŀ���� ǥ���մϴ�.
        HUD->ShowMissionClearOverlay(TotalEnemyNumber); // �̼� Ŭ���� �������̸� ǥ���մϴ�.
    }
}
