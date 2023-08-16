// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "InfinityLevelGameMode.generated.h"

class AEnemySpawner;
class AFantasyPlayerController;
class UMyGameInstance;
class ANormalLevelHUD;
class ABaseCharacter;
class ACountess;
class APhase;
class ATargetPoint;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AInfinityLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	AInfinityLevelGameMode();

	UFUNCTION()
	void SpawnPlayerCharacter();

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	void PlayerDie();

	UFUNCTION()
	void EnemyDie();

	UFUNCTION()
	int32 PickRandomEnemy();

	UFUNCTION()
	int32 PIckRandomSpawner();

	UPROPERTY(VisibleAnywhere)
	TArray<ATargetPoint*> TargetPointList;

	FORCEINLINE int32 GetSlayedEnemyNumber() const { return SlayedEnemyNumber; }
	FORCEINLINE int32 GetLeftEnemyNumber() const { return LeftEnemyNumber; }

protected:

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;


private:

	UPROPERTY()
	AFantasyPlayerController* PlayerController;

	UPROPERTY()
	UMyGameInstance* GameInstance;

	UPROPERTY()
	TArray<AEnemySpawner*> EnemySpawner;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy>> EnemyList;

	UPROPERTY()
	ANormalLevelHUD* HUD;

	UPROPERTY()
	float TimeSinceLastDecrease = 0.0f;

	UPROPERTY(EditAnywhere)
	float SpawnCycle = 10.f;

	UPROPERTY(EditAnywhere)
	int32 LeftEnemyNumber;

	UPROPERTY(EditAnywhere)
	int32 MaxEnemyNumber = 5;

	UPROPERTY(EditAnywhere)
	int32 SlayedEnemyNumber = 0;

	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
	USoundBase* BackGroundMusic;

	UPROPERTY()
	UAudioComponent* BGMComponent;

	UPROPERTY(EditAnywhere, Category = "CharacterClass")
	TSubclassOf<ACountess> CountessClass;

	UPROPERTY(EditAnywhere, Category = "CharacterClass")
	TSubclassOf<APhase> PhaseClass;

	UPROPERTY()
	ABaseCharacter* Character;

	

};
