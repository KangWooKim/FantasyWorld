// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NormalLevelHUD.generated.h"

class UMissionClearOverlay;
class UDieMenuOverlay;
class AFantasyPlayerController;
class UMissionPreviewOverlay;
class UCombatOverlay;
class USettingsOverlay;
class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ANormalLevelHUD : public AHUD
{
	GENERATED_BODY()

	
public:

	// ANormalLevelHUD의 생성자 선언
	ANormalLevelHUD();
	// BeginPlay() 함수를 오버라이딩. 게임이 실행되면 처음으로 호출됨.
	virtual void BeginPlay() override;

	// 미션 완료 오버레이를 생성하는 함수
	UFUNCTION()
		void CreateMissionClearOverlay();

	// 미션 완료 오버레이를 표시하는 함수
	// SlayedEnemy: 플레이어가 죽인 적의 수
	UFUNCTION()
		void ShowMissionClearOverlay(int32 SlayedEnemy);

	// 미션 완료 오버레이를 숨기는 함수
	UFUNCTION()
		void HideMissionClearOverlay();

	// 죽음 메뉴 오버레이를 생성하는 함수
	UFUNCTION()
		void CreateDieMenuOverlay();

	// 죽음 메뉴 오버레이를 표시하는 함수
	// SlayedEnemy: 플레이어가 죽기 전에 죽인 적의 수
	// LeftEnemy: 플레이어가 죽은 후 남은 적의 수
	UFUNCTION()
		void ShowDieMenuOverlay(int32 SlayedEnemy, int32 LeftEnemy);

	// 죽음 메뉴 오버레이를 숨기는 함수
	UFUNCTION()
		void HideDieMenuOverlay();

	// 미션 프리뷰 오버레이를 제거하는 함수
	UFUNCTION()
		void RemoveMissionPreviewOverlay();

	// 전투 오버레이를 생성하는 함수
	UFUNCTION()
		void CreateCombatOverlay();

	// 전투 오버레이를 표시하는 함수
	UFUNCTION()
		void ShowCombatOverlay();

	// 전투 오버레이를 숨기는 함수
	UFUNCTION()
		void HideCombatOverlay();

	// 설정 오버레이를 생성하는 함수
	UFUNCTION()
		void CreateSettingsOverlay();

	// 설정 오버레이를 표시하는 함수
	UFUNCTION()
		void ShowSettingsOverlay();

	// 설정 오버레이를 숨기는 함수
	UFUNCTION()
		void HideSettingsOverlay();

	// CombatOverlay를 반환하는 인라인 함수, FORCEINLINE은 컴파일러에 인라인 확장을 강제
	UFUNCTION()
		FORCEINLINE UCombatOverlay* GetCombatOverlay() { return CombatOverlay; }


protected:


	// 클래스의 private 섹션 시작
private:

	// UWorld 객체에 대한 포인터. 현재 게임의 세계를 나타냄.
	UPROPERTY()
		UWorld* World;

	// AFantasyPlayerController에 대한 포인터. 플레이어의 컨트롤을 관리.
	UPROPERTY()
		AFantasyPlayerController* PlayerController;

	// UMyGameInstance에 대한 포인터. 게임의 전반적인 상태나 설정을 관리.
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// 미션 완료 오버레이에 대한 포인터.
	UPROPERTY()
		UMissionClearOverlay* MissionClearOverlay;

	// 미션 완료 오버레이 클래스의 UClass 타입.
	// EditDefaultsOnly: 이 변수는 에디터에서만 기본값을 수정할 수 있음.
	// Category = "Overlay": 에디터에서 이 변수가 "Overlay" 카테고리에 속하도록 설정.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UMissionClearOverlay> MissionClearOverlayClass;

	// 죽음 메뉴 오버레이에 대한 포인터.
	UPROPERTY()
		UDieMenuOverlay* DieMenuOverlay;

	// 죽음 메뉴 오버레이 클래스의 UClass 타입.
	// EditDefaultsOnly와 Category는 위와 동일.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UDieMenuOverlay> DieMenuOverlayClass;

	// 미션 프리뷰 오버레이에 대한 포인터.
	UPROPERTY()
		UMissionPreviewOverlay* MissionPreviewOverlay;

	// 미션 프리뷰 오버레이 클래스의 UClass 타입.
	// EditDefaultsOnly와 Category는 위와 동일.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UMissionPreviewOverlay> MissionPreviewOverlayClass;

	// 전투 오버레이에 대한 포인터.
	UPROPERTY()
		UCombatOverlay* CombatOverlay;

	// 전투 오버레이 클래스의 UClass 타입.
	// EditDefaultsOnly와 Category는 위와 동일.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UCombatOverlay> CombatOverlayClass;

	// 설정 오버레이에 대한 포인터.
	UPROPERTY()
		USettingsOverlay* SettingsOverlay;

	// 설정 오버레이 클래스의 UClass 타입.
	// EditDefaultsOnly와 Category는 위와 동일.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<USettingsOverlay> SettingsOverlayClass;

	
};
