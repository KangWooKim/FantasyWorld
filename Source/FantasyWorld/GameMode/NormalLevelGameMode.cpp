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

// 기본 게임 모드 클래스를 상속받는 ANormalLevelGameMode의 생성자입니다.
ANormalLevelGameMode::ANormalLevelGameMode()
{
}

// 게임의 초기 설정을 처리하는 BeginPlay 메서드입니다.
void ANormalLevelGameMode::BeginPlay()
{
    // 부모 클래스의 BeginPlay 메서드를 호출합니다.
    Super::BeginPlay();

    // 현재 월드를 가져와서 변수에 저장합니다.
    World = GetWorld();

    // GameInstance가 null인 경우 현재 게임 인스턴스를 가져와서 변수에 저장합니다.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // PlayerController가 null인 경우 현재 플레이어 컨트롤러를 가져와서 변수에 저장합니다.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

    // PlayerController가 null이 아닐 경우, HUD를 설정합니다.
    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
    }

    // 배경 음악을 위한 오디오 컴포넌트를 생성하고 설정합니다.
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

    // 플레이어 캐릭터를 생성합니다.
    SpawnPlayerCharacter();

    // 월드에 존재하는 적을 모두 가져와서 EnemyList에 저장합니다.
    TArray<AActor*> EnemyList;
    UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), EnemyList);

    // 적의 수를 설정합니다.
    LeftEnemyNumber = EnemyList.Num();
    TotalEnemyNumber = EnemyList.Num();

    // 각 적의 EnemyDeath 이벤트에 대한 핸들러를 설정합니다.
    for (AActor* Enemy : EnemyList) {
        ABaseEnemy* E = Cast<ABaseEnemy>(Enemy);
        E->EnemyDeath.AddDynamic(this, &ANormalLevelGameMode::EnemyDie);
    }
}

// 프레임마다 호출되는 Tick 메서드입니다.
void ANormalLevelGameMode::Tick(float DeltaTime)
{
    // 부모 클래스의 Tick 메서드를 호출합니다.
    Super::Tick(DeltaTime);

    // GameInstance가 null인 경우 현재 게임 인스턴스를 가져와서 변수에 저장합니다.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // 배경 음악의 볼륨을 업데이트합니다.
    BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());

    // PlayerController가 null인 경우 현재 플레이어 컨트롤러를 가져와서 변수에 저장합니다.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

    // PlayerController와 HUD가 null이 아닐 경우, HUD의 적 개수 텍스트를 업데이트합니다.
    if (PlayerController) {
        HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
        if (HUD) {
            HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, TotalEnemyNumber));
        }
    }
}


// 플레이어가 죽었을 때 호출되는 메서드입니다.
void ANormalLevelGameMode::PlayerDie()
{
    // PlayerController가 null인 경우 현재 플레이어 컨트롤러를 가져와서 변수에 저장합니다.
    PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

    // HUD가 null인 경우 현재 HUD를 가져와서 변수에 저장합니다.
    HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

    // 월드, PlayerController, HUD가 null이 아니라면 아래 로직을 수행합니다.
    if (World && PlayerController && HUD) {
        PlayerController->UnPossess(); // 플레이어의 캐릭터 점유를 해제합니다.
        PlayerController->SetShowMouseCursor(true); // 마우스 커서를 표시합니다.
        HUD->ShowDieMenuOverlay(TotalEnemyNumber - LeftEnemyNumber, LeftEnemyNumber); // 죽음 메뉴 오버레이를 표시합니다.
    }
}

// 적이 죽었을 때 호출되는 메서드입니다.
void ANormalLevelGameMode::EnemyDie()
{
    LeftEnemyNumber = LeftEnemyNumber - 1; // 남은 적의 수를 감소시킵니다.

    // 모든 적이 죽었을 경우 StageClear 메서드를 호출합니다.
    if (LeftEnemyNumber == 0) {
        StageClear();
    }
    // 남은 적의 수를 HUD에 업데이트합니다.
    HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, TotalEnemyNumber));
}

// 플레이어 캐릭터를 스폰하는 메서드입니다.
void ANormalLevelGameMode::SpawnPlayerCharacter()
{
    // GameInstance가 null인 경우 현재 게임 인스턴스를 가져와서 변수에 저장합니다.
    GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

    // 월드와 GameInstance가 null이 아니면 아래 로직을 수행합니다.
    if (GetWorld() && GameInstance) {
        FActorSpawnParameters SpawnParams;
        ECharacterTypes CharacterTypes = GameInstance->GetSelectedCharacter(); // 선택된 캐릭터의 타입을 가져옵니다.

        // 선택된 캐릭터에 따라 캐릭터를 스폰합니다.
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

    // 캐릭터가 성공적으로 스폰되었으면 플레이어 컨트롤러가 그 캐릭터를 점유합니다.
    if (Character) {
        PlayerController->Possess(Character);
        Character->OnDeathEnd.AddDynamic(this, &ANormalLevelGameMode::PlayerDie); // 캐릭터의 OnDeathEnd 이벤트에 대한 핸들러를 설정합니다.
    }
}

// 모든 적을 처치하여 스테이지를 클리어했을 때 호출되는 메서드입니다.
void ANormalLevelGameMode::StageClear()
{
    // HUD가 null인 경우 현재 HUD를 가져와서 변수에 저장합니다.
    HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

    // 월드, PlayerController, HUD가 null이 아니라면 아래 로직을 수행합니다.
    if (World && PlayerController && HUD) {
        PlayerController->UnPossess(); // 플레이어의 캐릭터 점유를 해제합니다.
        PlayerController->SetShowMouseCursor(true); // 마우스 커서를 표시합니다.
        HUD->ShowMissionClearOverlay(TotalEnemyNumber); // 미션 클리어 오버레이를 표시합니다.
    }
}
