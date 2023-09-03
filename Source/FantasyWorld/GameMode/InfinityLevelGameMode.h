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
	
		// �� Ŭ������ ���� ���� ���� ��忡 ���� ���Ǹ� �����մϴ�.
public:

	// �⺻ ������
	AInfinityLevelGameMode();

	// �÷��̾� ĳ���͸� �����ϴ� �Լ�
	UFUNCTION()
		void SpawnPlayerCharacter();

	// ���� �����ϴ� �Լ�
	UFUNCTION()
		void SpawnEnemy();

	// �÷��̾ ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void PlayerDie();

	// ���� ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void EnemyDie();

	// ������ ���� �����ϴ� �Լ�
	UFUNCTION()
		int32 PickRandomEnemy();

	// ������ �� �����ʸ� �����ϴ� �Լ�
	UFUNCTION()
		int32 PIckRandomSpawner();

	// Ÿ�� ����Ʈ ����Ʈ. AI�� �̵��� ���� �� ����
	UPROPERTY(VisibleAnywhere)
		TArray<ATargetPoint*> TargetPointList;

	// ���� ���� ���� ��ȯ�ϴ� �ζ��� �Լ�
	FORCEINLINE int32 GetSlayedEnemyNumber() const { return SlayedEnemyNumber; }

	// �����ִ� ���� ���� ��ȯ�ϴ� �ζ��� �Լ�
	FORCEINLINE int32 GetLeftEnemyNumber() const { return LeftEnemyNumber; }

protected:

	// Tick �Լ� �������̵�. �����Ӹ��� ȣ���
	// DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
	virtual void Tick(float DeltaTime) override;

	// BeginPlay �Լ� �������̵�. ���� �� �� �� ȣ���
	virtual void BeginPlay() override;

private:

	// �÷��̾� ��Ʈ�ѷ� ����
	UPROPERTY()
		AFantasyPlayerController* PlayerController;

	// ���� �ν��Ͻ� ����
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// �� ������ �迭
	UPROPERTY()
		TArray<AEnemySpawner*> EnemySpawner;

	// �� Ŭ���� ���
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AEnemy>> EnemyList;

	// HUD ����
	UPROPERTY()
		ANormalLevelHUD* HUD;

	// ���������� ���� ������ �� ����� �ð�
	UPROPERTY()
		float TimeSinceLastDecrease = 0.0f;

	// ���� �����ϴ� �ֱ� (��)
	UPROPERTY(EditAnywhere)
		float SpawnCycle = 10.f;

	// �����ִ� ���� ��
	UPROPERTY(EditAnywhere)
		int32 LeftEnemyNumber;

	// �ִ� ���� ��
	UPROPERTY(EditAnywhere)
		int32 MaxEnemyNumber = 5;

	// ���� ���� ��
	UPROPERTY(EditAnywhere)
		int32 SlayedEnemyNumber = 0;

	// ��� ����
	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
		USoundBase* BackGroundMusic;

	// ��� ������ ����ϱ� ���� ����� ������Ʈ
	UPROPERTY()
		UAudioComponent* BGMComponent;

	// Countess ĳ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<ACountess> CountessClass;

	// Phase ĳ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<APhase> PhaseClass;

	// ���� ĳ���� ����
	UPROPERTY()
		ABaseCharacter* Character;

};
