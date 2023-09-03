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




// 생성자: 이 클래스가 인스턴스화될 때 호출됩니다.
AInfinityLevelGameMode::AInfinityLevelGameMode()
{
    // 아무 것도 하지 않습니다. 기본 생성자입니다.
}

// 플레이어 캐릭터를 생성하는 함수
void AInfinityLevelGameMode::SpawnPlayerCharacter()
{
    // GameInstance가 없으면 현재의 GameInstance를 얻어옵니다.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // GetWorld()와 GameInstance가 유효한 경우
    if (GetWorld() && GameInstance) {
        FActorSpawnParameters SpawnParams;

        // GameInstance에서 선택된 캐릭터 타입을 가져옵니다.
        ECharacterTypes CharacterTypes = GameInstance->GetSelectedCharacter();

        // CountessClass와 PhaseClass가 유효한 경우
        if (CountessClass && PhaseClass) {
            TArray<AActor*> PlayerStarts;

            // 모든 PlayerStart 액터를 가져옵니다.
            UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

            // 선택된 캐릭터 타입에 따라 캐릭터를 생성합니다.
            if (CharacterTypes == ECharacterTypes::Countess) {
                Character = GetWorld()->SpawnActor<ACountess>(CountessClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
            }
            else if (CharacterTypes == ECharacterTypes::Phase) {
                Character = GetWorld()->SpawnActor<APhase>(PhaseClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
            }
        }
    }

    // 캐릭터가 생성되었으면, PlayerController로 캐릭터를 조종합니다.
    if (Character) {
        PlayerController->Possess(Character);

        // 캐릭터가 죽었을 때 호출할 함수를 설정합니다.
        Character->OnDeathEnd.AddDynamic(this, &AInfinityLevelGameMode::PlayerDie);
    }
}

// 적을 생성하는 함수
void AInfinityLevelGameMode::SpawnEnemy()
{
    const int32 Enemyindex = PickRandomEnemy();
    const int32 Spawnerindex = PIckRandomSpawner();
    FActorSpawnParameters SpawnParams;

    // 유효한 적과 스포너 인덱스를 가져옵니다.
    if (Enemyindex == -1 || Spawnerindex == -1) return;

    // 선택된 적과 스포너 위치에서 적을 생성합니다.
    AEnemy* SpawnEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyList[Enemyindex], EnemySpawner[Spawnerindex]->GetActorLocation(), EnemySpawner[Spawnerindex]->GetActorRotation(), SpawnParams);

    if (SpawnEnemy) {
        // 적을 제어할 AIController를 생성합니다.
        AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
        AIController->Possess(SpawnEnemy);

        // 적이 죽을 때 호출될 함수를 설정합니다.
        SpawnEnemy->EnemyDeath.AddDynamic(this, &AInfinityLevelGameMode::EnemyDie);

        // PatrolTarget에 랜덤한 TargetPoint를 할당합니다.
        if (TargetPointList.Num() > 0)
        {
            int32 RandomIndex = FMath::RandRange(0, TargetPointList.Num() - 1);
            SpawnEnemy->PatrolTarget = EnemySpawner[Spawnerindex];

            // 여러 랜덤한 TargetPoint를 선택하여 PatrolTargets 배열에 할당합니다.
            for (int i = 0; i < 3; i++)
            {
                RandomIndex = FMath::RandRange(0, TargetPointList.Num() - 1);
                SpawnEnemy->PatrolTargets.Add(TargetPointList[RandomIndex]);
            }
        }

        // 남은 적의 수를 증가시키고 HUD에 표시합니다.
        LeftEnemyNumber++;
        HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, MaxEnemyNumber));
    }
}

// 플레이어가 죽었을 때 호출되는 함수
void AInfinityLevelGameMode::PlayerDie()
{
    // PlayerController와 HUD가 없으면 현재의 World에서 가져옵니다.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
    HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

    // 유효한 경우에만 실행합니다.
    if (GetWorld() && PlayerController && HUD) {
        // 플레이어 캐릭터의 조종을 중지합니다.
        PlayerController->UnPossess();

        // 마우스 커서를 표시합니다.
        PlayerController->SetShowMouseCursor(true);

        // 죽은 플레이어에 대한 메뉴를 표시합니다.
        HUD->ShowDieMenuOverlay(SlayedEnemyNumber, LeftEnemyNumber);
    }
}


// 적이 죽었을 때 호출되는 함수
void AInfinityLevelGameMode::EnemyDie()
{
    // 적이 죽으면 남은 적의 수를 하나 줄입니다.
    LeftEnemyNumber = LeftEnemyNumber - 1;
    // 죽은 적의 수를 하나 늘립니다.
    SlayedEnemyNumber += 1;
    // 남은 적의 수를 HUD에 업데이트합니다.
    HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, MaxEnemyNumber));
}

// 랜덤한 적을 선택하는 함수
int32 AInfinityLevelGameMode::PickRandomEnemy()
{
    // 적 리스트가 비어있으면 -1을 반환합니다.
    if (EnemyList.Num() == 0) return -1;

    int32 index;
    if (EnemyList.Num() > 0) {
        const int32 MaxRange = EnemyList.Num() - 1;
        // 랜덤 인덱스를 생성합니다.
        index = FMath::RandRange(0, MaxRange);
    }
    return index;
}

// 랜덤한 스포너를 선택하는 함수
int32 AInfinityLevelGameMode::PIckRandomSpawner()
{
    // 스포너 리스트가 비어있으면 -1을 반환합니다.
    if (EnemySpawner.Num() == 0) return -1;

    const int MaxRange = EnemySpawner.Num() - 1;
    // 랜덤 인덱스를 생성합니다.
    const int index = FMath::FRandRange(0, MaxRange);
    return index;
}

// Tick 함수: 게임 로직을 주기적으로 업데이트합니다.
void AInfinityLevelGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 마지막으로 적이 스폰된 이후의 시간을 측정합니다.
    TimeSinceLastDecrease += DeltaTime;

    // 일정 시간이 지난 후에 적을 스폰합니다.
    if (TimeSinceLastDecrease >= SpawnCycle) {
        TimeSinceLastDecrease -= SpawnCycle;
        if (LeftEnemyNumber < MaxEnemyNumber) {
            SpawnEnemy();
        }
    }

    // 필요한 객체들을 초기화하거나 가져옵니다.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;

    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
    }

    // BGM 볼륨을 설정합니다.
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
}

// 게임이 시작될 때 호출되는 함수
void AInfinityLevelGameMode::BeginPlay()
{
    Super::BeginPlay();

    // 필요한 객체들을 초기화하거나 가져옵니다.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;

    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
    }

    // BGM 컴포넌트를 생성하고 설정합니다.
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

    // 플레이어 캐릭터를 스폰합니다.
    SpawnPlayerCharacter();

    // 게임 월드 내의 모든 적을 담을 배열을 선언합니다.
    TArray<AActor*> Enemies;
    // 게임 월드에 존재하는 모든 'AEnemy' 타입의 액터를 가져와 'Enemies' 배열에 담습니다.
    UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enemies);
    // 현재 게임 월드에 존재하는 적의 수를 'LeftEnemyNumber'에 저장합니다.
    LeftEnemyNumber = Enemies.Num();

    // 각 적에 대해 루프를 돌며 'EnemyDie' 함수를 죽음 이벤트에 바인딩합니다.
    for (AActor* Enemy : Enemies) {
        ABaseEnemy* E = Cast<ABaseEnemy>(Enemy);  // AActor를 ABaseEnemy로 캐스팅합니다.
        E->EnemyDeath.AddDynamic(this, &AInfinityLevelGameMode::EnemyDie);  // 죽음 이벤트에 'EnemyDie' 함수를 바인딩합니다.
    }

    // 게임 월드 내의 모든 적 스포너를 담을 배열을 선언합니다.
    TArray<AActor*> List;
    // 게임 월드에 존재하는 모든 'AEnemySpawner' 타입의 액터를 가져와 'List' 배열에 담습니다.
    UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), List);

    // 각 스포너에 대해 루프를 돌며 'EnemySpawner' 배열에 추가합니다.
    for (AActor* L : List) {
        AEnemySpawner* Spawner = Cast<AEnemySpawner>(L);  // AActor를 AEnemySpawner로 캐스팅합니다.
        EnemySpawner.Add(Spawner);  // 스포너를 'EnemySpawner' 배열에 추가합니다.
    }

    // 게임 월드에서 모든 'ATargetPoint'를 찾아 'TargetPointList' 배열에 추가합니다.
    for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It) {
        ATargetPoint* TargetPoint = *It;  // 반복자에서 ATargetPoint를 가져옵니다.
        if (TargetPoint) {
            TargetPointList.Add(TargetPoint);  // 유효한 'ATargetPoint'를 배열에 추가합니다.
        }
    }

}
