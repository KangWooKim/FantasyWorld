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




AInfinityLevelGameMode::AInfinityLevelGameMode()
{
}

void AInfinityLevelGameMode::SpawnPlayerCharacter()
{
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	if (GetWorld() && GameInstance) {
		FActorSpawnParameters SpawnParams;
		ECharacterTypes CharacterTypes = GameInstance->GetSelectedCharacter();
		if (CountessClass && PhaseClass) {
			TArray<AActor*> PlayerStarts;
			UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
			if (CharacterTypes == ECharacterTypes::Countess) {
				Character = GetWorld()->SpawnActor<ACountess>(CountessClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
			}
			else if (CharacterTypes == ECharacterTypes::Phase) {
				Character = GetWorld()->SpawnActor<APhase>(PhaseClass, PlayerStarts[0]->GetActorLocation(), PlayerStarts[0]->GetActorRotation(), SpawnParams);
			}
		}
	}
	if (Character) {
		PlayerController->Possess(Character);
		Character->OnDeathEnd.AddDynamic(this, &AInfinityLevelGameMode::PlayerDie);
	}
}

void AInfinityLevelGameMode::SpawnEnemy()
{
	const int32 Enemyindex = PickRandomEnemy();
	const int32 Spawnerindex = PIckRandomSpawner();
	FActorSpawnParameters SpawnParams;
	if (Enemyindex == -1 || Spawnerindex == -1) return;
	AEnemy* SpawnEnemy = GetWorld()->SpawnActor<AEnemy>(EnemyList[Enemyindex], EnemySpawner[Spawnerindex]->GetActorLocation(), EnemySpawner[Spawnerindex]->GetActorRotation(), SpawnParams); 
	
	if (SpawnEnemy) {
		AAIController* AIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
		AIController->Possess(SpawnEnemy);

		SpawnEnemy->EnemyDeath.AddDynamic(this, &AInfinityLevelGameMode::EnemyDie);
		if (TargetPointList.Num() > 0)
		{
			// 랜덤한 TargetPoint를 선택하여 PatrolTarget에 할당합니다.
			int32 RandomIndex = FMath::RandRange(0, TargetPointList.Num() - 1);
			SpawnEnemy->PatrolTarget = EnemySpawner[Spawnerindex];

			// 여러 랜덤한 TargetPoint를 선택하여 PatrolTargets 배열에 할당합니다.
			// 이 예에서는 3개의 랜덤한 TargetPoint를 선택합니다.
			for (int i = 0; i < 3; i++)
			{
				RandomIndex = FMath::RandRange(0, TargetPointList.Num() - 1);
				SpawnEnemy->PatrolTargets.Add(TargetPointList[RandomIndex]);
			}
		}
		LeftEnemyNumber++;
		HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, MaxEnemyNumber));
	}
}

void AInfinityLevelGameMode::PlayerDie()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (GetWorld() && PlayerController && HUD) {
		PlayerController->UnPossess();
		PlayerController->SetShowMouseCursor(true);
		HUD->ShowDieMenuOverlay(SlayedEnemyNumber, LeftEnemyNumber);
	}
}

void AInfinityLevelGameMode::EnemyDie()
{
	LeftEnemyNumber = LeftEnemyNumber - 1;
	SlayedEnemyNumber += 1;
	HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, MaxEnemyNumber));
}

int32 AInfinityLevelGameMode::PickRandomEnemy()
{
	if (EnemyList.Num() == 0) return -1;
	int32 index;
	if (EnemyList.Num() > 0) {
		const int32 MaxRange = EnemyList.Num() - 1;
		index = FMath::RandRange(0, MaxRange);
	}
	return index;
}

int32 AInfinityLevelGameMode::PIckRandomSpawner()
{
	if (EnemySpawner.Num() == 0) return -1;
	const int MaxRange = EnemySpawner.Num() - 1;
	const int index = FMath::FRandRange(0, MaxRange);
	return index;
}

void AInfinityLevelGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TimeSinceLastDecrease += DeltaTime;

	if (TimeSinceLastDecrease >= SpawnCycle) {
		TimeSinceLastDecrease -= SpawnCycle;
		if (LeftEnemyNumber < MaxEnemyNumber) {
			SpawnEnemy();
		}
	}

	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;

	if (PlayerController) {
		HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	}
}

void AInfinityLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : PlayerController;
	if (PlayerController) {
		HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	}

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

	SpawnPlayerCharacter();

	TArray<AActor*> Enemies;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), Enemies);
	LeftEnemyNumber = Enemies.Num();

	for (AActor* Enemy : Enemies) {
		ABaseEnemy* E = Cast<ABaseEnemy>(Enemy);
		E->EnemyDeath.AddDynamic(this, &AInfinityLevelGameMode::EnemyDie);
	}

	TArray<AActor*> List;

	UGameplayStatics::GetAllActorsOfClass(this, AEnemySpawner::StaticClass(), List);

	for (AActor* L : List) {
		AEnemySpawner* Spawner = Cast<AEnemySpawner>(L);
		EnemySpawner.Add(Spawner);
	}
	
	for (TActorIterator<ATargetPoint> It(GetWorld()); It; ++It)
	{
		ATargetPoint* TargetPoint = *It;
		if (TargetPoint)
		{
			TargetPointList.Add(TargetPoint);
		}
	}
}
