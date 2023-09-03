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

	// �⺻ ������ ����
	ANormalLevelGameMode();

	// BeginPlay �޼��� �������̵� ����. ������ ���۵� �� ȣ��˴ϴ�.
	virtual void BeginPlay() override;

	// Tick �޼��� �������̵� ����. �����Ӹ��� ȣ��˴ϴ�.
	// DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
	virtual void Tick(float DeltaTime) override;

	// �÷��̾� ĳ���͸� �����ϴ� �Լ�
	UFUNCTION()
		void SpawnPlayerCharacter();

	// ���� ���� ���� ��ȯ�ϴ� �ζ��� �Լ�
	FORCEINLINE int32 GetLeftEnemyNumber() { return LeftEnemyNumber; }

	// �� ���� ���� ��ȯ�ϴ� �ζ��� �Լ�
	FORCEINLINE int32 GetTotalEnemyNumber() { return TotalEnemyNumber; }

	// �÷��̾ �׾��� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void PlayerDie();

	// ���� �׾��� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void EnemyDie();

	// ���������� Ŭ�������� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void StageClear();

protected:

private:

	// ���� �ν��Ͻ��� ���� ����
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// �÷��̾� ��Ʈ�ѷ��� ���� ����
	UPROPERTY()
		AFantasyPlayerController* PlayerController;

	// HUD�� ���� ����
	UPROPERTY()
		ANormalLevelHUD* HUD;

	// ���忡 ���� ����
	UPROPERTY()
		UWorld* World;

	// �� ���� ��
	UPROPERTY(EditAnywhere)
		int32 TotalEnemyNumber;

	// ���� ���� ��
	UPROPERTY(EditAnywhere)
		int32 LeftEnemyNumber;

	// Countess ĳ���� Ŭ������ ���� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<ACountess> CountessClass;

	// Phase ĳ���� Ŭ������ ���� ���� Ŭ����
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<APhase> PhaseClass;

	// ĳ���Ϳ� ���� ����
	UPROPERTY()
		ABaseCharacter* Character;

	// ������ǿ� ���� ����
	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
		USoundBase* BackGroundMusic;

	// ��������� ����ϱ� ���� ����� ������Ʈ
	UPROPERTY()
		UAudioComponent* BGMComponent;

};
