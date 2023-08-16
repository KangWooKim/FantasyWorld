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

ANormalLevelGameMode::ANormalLevelGameMode()
{
}

void ANormalLevelGameMode::BeginPlay()
{
	Super::BeginPlay();

	World = GetWorld();

	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
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
	
	TArray<AActor*> EnemyList;
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), EnemyList);
	LeftEnemyNumber = EnemyList.Num();
	TotalEnemyNumber = EnemyList.Num();
	
	for (AActor* Enemy : EnemyList) {
		ABaseEnemy* E = Cast<ABaseEnemy>(Enemy);
		E->EnemyDeath.AddDynamic(this, &ANormalLevelGameMode::EnemyDie);
	}
}

void ANormalLevelGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	BGMComponent->SetVolumeMultiplier(GameInstance->GetBGMVolume());
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	
	if (PlayerController) {
		HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD) {
			HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, TotalEnemyNumber));
		}
	}

}

void ANormalLevelGameMode::PlayerDie()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (World && PlayerController && HUD) {
		PlayerController->UnPossess();
		PlayerController->SetShowMouseCursor(true);
		HUD->ShowDieMenuOverlay(TotalEnemyNumber - LeftEnemyNumber, LeftEnemyNumber);
	}
}

void ANormalLevelGameMode::EnemyDie() 
{
	LeftEnemyNumber = LeftEnemyNumber - 1;
	if (LeftEnemyNumber == 0) {
		StageClear();
	}
	HUD->GetCombatOverlay()->UpdateLeftEnemyText(FMath::Clamp(LeftEnemyNumber, 0, TotalEnemyNumber));
}

void ANormalLevelGameMode::SpawnPlayerCharacter()
{
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	if (GetWorld() && GameInstance) {
		FActorSpawnParameters SpawnParams;
		ECharacterTypes CharacterTypes = GameInstance->GetSelectedCharacter();
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
	if (Character) {
		PlayerController->Possess(Character);
		Character->OnDeathEnd.AddDynamic(this, &ANormalLevelGameMode::PlayerDie);
	}
}

void ANormalLevelGameMode::StageClear()
{
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (World && PlayerController && HUD) {
		PlayerController->UnPossess();
		PlayerController->SetShowMouseCursor(true);
		HUD->ShowMissionClearOverlay(TotalEnemyNumber);
	}
}
