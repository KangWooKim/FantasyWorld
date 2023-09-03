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
	
		// 이 클래스는 무한 레벨 게임 모드에 대한 정의를 포함합니다.
public:

	// 기본 생성자
	AInfinityLevelGameMode();

	// 플레이어 캐릭터를 스폰하는 함수
	UFUNCTION()
		void SpawnPlayerCharacter();

	// 적을 스폰하는 함수
	UFUNCTION()
		void SpawnEnemy();

	// 플레이어가 죽을 때 호출되는 함수
	UFUNCTION()
		void PlayerDie();

	// 적이 죽을 때 호출되는 함수
	UFUNCTION()
		void EnemyDie();

	// 랜덤한 적을 선택하는 함수
	UFUNCTION()
		int32 PickRandomEnemy();

	// 랜덤한 적 스포너를 선택하는 함수
	UFUNCTION()
		int32 PIckRandomSpawner();

	// 타겟 포인트 리스트. AI의 이동에 사용될 수 있음
	UPROPERTY(VisibleAnywhere)
		TArray<ATargetPoint*> TargetPointList;

	// 죽인 적의 수를 반환하는 인라인 함수
	FORCEINLINE int32 GetSlayedEnemyNumber() const { return SlayedEnemyNumber; }

	// 남아있는 적의 수를 반환하는 인라인 함수
	FORCEINLINE int32 GetLeftEnemyNumber() const { return LeftEnemyNumber; }

protected:

	// Tick 함수 오버라이드. 프레임마다 호출됨
	// DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
	virtual void Tick(float DeltaTime) override;

	// BeginPlay 함수 오버라이드. 시작 시 한 번 호출됨
	virtual void BeginPlay() override;

private:

	// 플레이어 컨트롤러 참조
	UPROPERTY()
		AFantasyPlayerController* PlayerController;

	// 게임 인스턴스 참조
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// 적 스포너 배열
	UPROPERTY()
		TArray<AEnemySpawner*> EnemySpawner;

	// 적 클래스 목록
	UPROPERTY(EditAnywhere)
		TArray<TSubclassOf<AEnemy>> EnemyList;

	// HUD 참조
	UPROPERTY()
		ANormalLevelHUD* HUD;

	// 마지막으로 적을 스폰한 후 경과한 시간
	UPROPERTY()
		float TimeSinceLastDecrease = 0.0f;

	// 적을 스폰하는 주기 (초)
	UPROPERTY(EditAnywhere)
		float SpawnCycle = 10.f;

	// 남아있는 적의 수
	UPROPERTY(EditAnywhere)
		int32 LeftEnemyNumber;

	// 최대 적의 수
	UPROPERTY(EditAnywhere)
		int32 MaxEnemyNumber = 5;

	// 죽인 적의 수
	UPROPERTY(EditAnywhere)
		int32 SlayedEnemyNumber = 0;

	// 배경 음악
	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
		USoundBase* BackGroundMusic;

	// 배경 음악을 재생하기 위한 오디오 컴포넌트
	UPROPERTY()
		UAudioComponent* BGMComponent;

	// Countess 캐릭터 클래스
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<ACountess> CountessClass;

	// Phase 캐릭터 클래스
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<APhase> PhaseClass;

	// 현재 캐릭터 참조
	UPROPERTY()
		ABaseCharacter* Character;

};
