// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NormalLevelGameMode.generated.h"

class UMyGameInstance;
class ACountess;
class APhase;
class ABaseCharacter;
class USoundBase;
class AFantasyPlayerController;
class ANormalLevelHUD;


/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ANormalLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	ANormalLevelGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SpawnPlayerCharacter();

	FORCEINLINE int32 GetLeftEnemyNumber() { return LeftEnemyNumber; }
	FORCEINLINE int32 GetTotalEnemyNumber() { return TotalEnemyNumber; }
	
	UFUNCTION()
	void PlayerDie();

	UFUNCTION()
	void EnemyDie();

	UFUNCTION()
	void StageClear();

protected:


private:

	UPROPERTY()
	UMyGameInstance* GameInstance;

	UPROPERTY()
	AFantasyPlayerController* PlayerController;

	UPROPERTY()
	ANormalLevelHUD* HUD;

	UPROPERTY()
	UWorld* World;

	UPROPERTY(EditAnywhere)
	int32 TotalEnemyNumber;

	UPROPERTY(EditAnywhere)
	int32 LeftEnemyNumber;

	UPROPERTY(EditAnywhere, Category = "CharacterClass")
	TSubclassOf<ACountess> CountessClass;

	UPROPERTY(EditAnywhere, Category = "CharacterClass")
	TSubclassOf<APhase> PhaseClass;

	UPROPERTY()
	ABaseCharacter* Character;

	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
	USoundBase* BackGroundMusic;

	UPROPERTY()
	UAudioComponent* BGMComponent;
};
