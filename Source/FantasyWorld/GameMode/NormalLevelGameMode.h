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

	// 기본 생성자 선언
	ANormalLevelGameMode();

	// BeginPlay 메서드 오버라이딩 선언. 게임이 시작될 때 호출됩니다.
	virtual void BeginPlay() override;

	// Tick 메서드 오버라이딩 선언. 프레임마다 호출됩니다.
	// DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
	virtual void Tick(float DeltaTime) override;

	// 플레이어 캐릭터를 스폰하는 함수
	UFUNCTION()
		void SpawnPlayerCharacter();

	// 남은 적의 수를 반환하는 인라인 함수
	FORCEINLINE int32 GetLeftEnemyNumber() { return LeftEnemyNumber; }

	// 총 적의 수를 반환하는 인라인 함수
	FORCEINLINE int32 GetTotalEnemyNumber() { return TotalEnemyNumber; }

	// 플레이어가 죽었을 때 호출되는 함수
	UFUNCTION()
		void PlayerDie();

	// 적이 죽었을 때 호출되는 함수
	UFUNCTION()
		void EnemyDie();

	// 스테이지를 클리어했을 때 호출되는 함수
	UFUNCTION()
		void StageClear();

protected:

private:

	// 게임 인스턴스에 대한 참조
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// 플레이어 컨트롤러에 대한 참조
	UPROPERTY()
		AFantasyPlayerController* PlayerController;

	// HUD에 대한 참조
	UPROPERTY()
		ANormalLevelHUD* HUD;

	// 월드에 대한 참조
	UPROPERTY()
		UWorld* World;

	// 총 적의 수
	UPROPERTY(EditAnywhere)
		int32 TotalEnemyNumber;

	// 남은 적의 수
	UPROPERTY(EditAnywhere)
		int32 LeftEnemyNumber;

	// Countess 캐릭터 클래스에 대한 서브 클래스
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<ACountess> CountessClass;

	// Phase 캐릭터 클래스에 대한 서브 클래스
	UPROPERTY(EditAnywhere, Category = "CharacterClass")
		TSubclassOf<APhase> PhaseClass;

	// 캐릭터에 대한 참조
	UPROPERTY()
		ABaseCharacter* Character;

	// 배경음악에 대한 참조
	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
		USoundBase* BackGroundMusic;

	// 배경음악을 재생하기 위한 오디오 컴포넌트
	UPROPERTY()
		UAudioComponent* BGMComponent;

};
