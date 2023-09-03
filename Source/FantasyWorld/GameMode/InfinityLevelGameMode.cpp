// Fill out your copyright notice in the Description page of Project Settings.


#include "InfinityLevelGameMode.h"
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
#include "FantasyWorld/EnemySpawner.h"
#include "Engine/TargetPoint.h"
#include "EngineUtils.h"
#include "AIController.h"




// ������: �� Ŭ������ �ν��Ͻ�ȭ�� �� ȣ��˴ϴ�.
AInfinityLevelGameMode::AInfinityLevelGameMode()
{
    // �ƹ� �͵� ���� �ʽ��ϴ�. �⺻ �������Դϴ�.
}

// �÷��̾� ĳ���͸� �����ϴ� �Լ�
void AInfinityLevelGameMode::SpawnPlayerCharacter()
{
    // GameInstance�� ������ ������ GameInstance�� ���ɴϴ�.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // GetWorld()�� GameInstance�� ��ȿ�� ���
    if (GetWorld() && GameInstance) {
        FActorSpawnParameters SpawnParams;

        // GameInstance���� ���õ� ĳ���� Ÿ���� �����ɴϴ�.
        ECharacterTypes CharacterTypes = GameInstance->GetSelectedCharacter();

        // CountessClass�� PhaseClass�� ��ȿ�� ���
        if (CountessClass && PhaseClass) {
            TArray<AActor*> PlayerStarts;

            // ��� PlayerStart ���͸� �����ɴϴ�.
            UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

            // ���õ� ĳ���� Ÿ�Կ� ���� ĳ���͸� �����մϴ�.
            if (CharacterTypes == ECharacterTypes::Countess) {
                Character = GetWorld()->SpawnActor<ACountess>(CountessClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
            }
            else if (CharacterTypes == ECharacterTypes::Phase) {
                Character = GetWorld()->SpawnActor<APhase>(PhaseClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
            }
        }
    }

    // ĳ���Ͱ� �����Ǿ�����, PlayerController�� ĳ���͸� �����մϴ�.
    if (Character) {
        PlayerController->Possess(Character);

        // ĳ���Ͱ� �׾��� �� ȣ���� �Լ��� �����մϴ�.
        Character->OnDeathEnd.AddDynamic(this, &AInfinityLevelGameMode::PlayerDie);
    }
}

// ���� �����ϴ� �Լ�
void AInfinityLevelGameMode::SpawnEnemy()
{
    const int32 Enemyindex = PickRandomEnemy();
    const int32 Spawnerindex = PIckRandomSpawner();
    FActorSpawnParameters SpawnParams;

    // ��ȿ�� ���� ������ �ε����� �����ɴϴ�.
    if (Enemyindex == -1 || Spawnerindex == -1) return;

    // ���õ� ���� ������ ��ġ���� ���� �����մϴ�.
    AEnemy* SpawnEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyList[Enemyindex], EnemySpawner[Spawnerindex]->GetActorLocation(), EnemySpawner[Spawnerindex]->GetActorRotation(), SpawnParams);

    if (SpawnEnemy) {
        // ���� ������ AIController�� �����մϴ�.
        AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
        AIController->Possess(SpawnEnemy);

        // ���� ���� �� ȣ��� �Լ��� �����մϴ�.
        SpawnEnemy->EnemyDeath.AddDynamic(this, &AInfinityLevelGameMode::EnemyDie);

        // PatrolTarget�� ������ TargetPoint�� �Ҵ��մϴ�.
        if (TargetPointList.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, TargetPointList.Num() - 1);
            SpawnEnemy->PatrolTarget = EnemySpawner[Spawnerindex];

            // ���� ������ TargetPoint�� �����Ͽ� PatrolTargets �迭�� �Ҵ��մϴ�.
            for (int i = 0; i < 3; i++)
            {
                RandomIndex = FMath::RandRange(0, TargetPointList.Num() - 1);
                SpawnEnemy->PatrolTargets.Add(TargetPointList[RandomIndex]);
            }
        }

        // ���� ���� ���� ������Ű�� HUD�� ǥ���մϴ�.
        LeftEnemyNumber++;
        HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, MaxEnemyNumber));
    }
}

// �÷��̾ �׾��� �� ȣ��Ǵ� �Լ�
void AInfinityLevelGameMode::PlayerDie()
{
    // PlayerController�� HUD�� ������ ������ World���� �����ɴϴ�.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
    HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

    // ��ȿ�� ��쿡�� �����մϴ�.
    if (GetWorld() && PlayerController && HUD) {
        // �÷��̾� ĳ������ ������ �����մϴ�.
        PlayerController->UnPossess();

        // ���콺 Ŀ���� ǥ���մϴ�.
        PlayerController->SetShowMouseCursor(true);

        // ���� �÷��̾ ���� �޴��� ǥ���մϴ�.
        HUD->ShowDieMenuOverlay(SlayedEnemyNumber, LeftEnemyNumber);
    }
}


// ���� �׾��� �� ȣ��Ǵ� �Լ�
void AInfinityLevelGameMode::EnemyDie()
{
    // ���� ������ ���� ���� ���� �ϳ� ���Դϴ�.
    LeftEnemyNumber = LeftEnemyNumber - 1;
    // ���� ���� ���� �ϳ� �ø��ϴ�.
    SlayedEnemyNumber += 1;
    // ���� ���� ���� HUD�� ������Ʈ�մϴ�.
    HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, MaxEnemyNumber));
}

// ������ ���� �����ϴ� �Լ�
int32 AInfinityLevelGameMode::PickRandomEnemy()
{
    // �� ����Ʈ�� ��������� -1�� ��ȯ�մϴ�.
    if (EnemyList.Num() == 0) return -1;

    int32 index;
    if (EnemyList.Num() > 0) {
        const int32 MaxRange = EnemyList.Num() - 1;
        // ���� �ε����� �����մϴ�.
        index = FMath::RandRange(0, MaxRange);
    }
    return index;
}

// ������ �����ʸ� �����ϴ� �Լ�
int32 AInfinityLevelGameMode::PIckRandomSpawner()
{
    // ������ ����Ʈ�� ��������� -1�� ��ȯ�մϴ�.
    if (EnemySpawner.Num() == 0) return -1;

    const int MaxRange = EnemySpawner.Num() - 1;
    // ���� �ε����� �����մϴ�.
    const int index = FMath::FRandRange(0, MaxRange);
    return index;
}

// Tick �Լ�: ���� ������ �ֱ������� ������Ʈ�մϴ�.
void AInfinityLevelGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���������� ���� ������ ������ �ð��� �����մϴ�.
    TimeSinceLastDecrease += DeltaTime;

    // ���� �ð��� ���� �Ŀ� ���� �����մϴ�.
    if (TimeSinceLastDecrease >= SpawnCycle) {
        TimeSinceLastDecrease -= SpawnCycle;
        if (LeftEnemyNumber < MaxEnemyNumber) {
            SpawnEnemy();
        }
    }

    // �ʿ��� ��ü���� �ʱ�ȭ�ϰų� �����ɴϴ�.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;

    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
    }

    // BGM ������ �����մϴ�.
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
}

// ������ ���۵� �� ȣ��Ǵ� �Լ�
void AInfinityLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    // �ʿ��� ��ü���� �ʱ�ȭ�ϰų� �����ɴϴ�.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;

    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
    }

    // BGM ������Ʈ�� �����ϰ� �����մϴ�.
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

    // ���� ���� ���� ��� ���� ���� �迭�� �����մϴ�.
    TArray<AActor*> Enemies;
    // ���� ���忡 �����ϴ� ��� 'AEnemy' Ÿ���� ���͸� ������ 'Enemies' �迭�� ����ϴ�.
    UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enemies);
    // ���� ���� ���忡 �����ϴ� ���� ���� 'LeftEnemyNumber'�� �����մϴ�.
    LeftEnemyNumber = Enemies.Num();

    // �� ���� ���� ������ ���� 'EnemyDie' �Լ��� ���� �̺�Ʈ�� ���ε��մϴ�.
    for (AActor* Enemy : Enemies) {
        ABaseEnemy* E = Cast<ABaseEnemy>(Enemy);  // AActor�� ABaseEnemy�� ĳ�����մϴ�.
        E->EnemyDeath.AddDynamic(this, &AInfinityLevelGameMode::EnemyDie);  // ���� �̺�Ʈ�� 'EnemyDie' �Լ��� ���ε��մϴ�.
    }

    // ���� ���� ���� ��� �� �����ʸ� ���� �迭�� �����մϴ�.
    TArray<AActor*> List;
    // ���� ���忡 �����ϴ� ��� 'AEnemySpawner' Ÿ���� ���͸� ������ 'List' �迭�� ����ϴ�.
    UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), List);

    // �� �����ʿ� ���� ������ ���� 'EnemySpawner' �迭�� �߰��մϴ�.
    for (AActor* L : List) {
        AEnemySpawner* Spawner = Cast<AEnemySpawner>(L);  // AActor�� AEnemySpawner�� ĳ�����մϴ�.
        EnemySpawner.Add(Spawner);  // �����ʸ� 'EnemySpawner' �迭�� �߰��մϴ�.
    }

    // ���� ���忡�� ��� 'ATargetPoint'�� ã�� 'TargetPointList' �迭�� �߰��մϴ�.
    for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It) {
        ATargetPoint* TargetPoint = *It;  // �ݺ��ڿ��� ATargetPoint�� �����ɴϴ�.
        if (TargetPoint) {
            TargetPointList.Add(TargetPoint);  // ��ȿ�� 'ATargetPoint'�� �迭�� �߰��մϴ�.
        }
    }

}
