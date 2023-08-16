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

	ANormalLevelHUD();
	virtual void BeginPlay() override;

	UFUNCTION()
	void CreateMissionClearOverlay();

	UFUNCTION()
	void ShowMissionClearOverlay(int32 SlayedEnemy);

	UFUNCTION()
	void HideMissionClearOverlay();

	UFUNCTION()
	void CreateDieMenuOverlay();

	UFUNCTION()
	void ShowDieMenuOverlay(int32 SlayedEnemy, int32 LeftEnemy);

	UFUNCTION()
	void HideDieMenuOverlay();

	UFUNCTION()
	void RemoveMissionPreviewOverlay();

	UFUNCTION()
	void CreateCombatOverlay();

	UFUNCTION()
	void ShowCombatOverlay();

	UFUNCTION()
	void HideCombatOverlay();

	UFUNCTION()
	void CreateSettingsOverlay();

	UFUNCTION()
	void ShowSettingsOverlay();

	UFUNCTION()
	void HideSettingsOverlay();

	UFUNCTION()
	FORCEINLINE UCombatOverlay* GetCombatOverlay() { return CombatOverlay; }

protected:


private:

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	AFantasyPlayerController* PlayerController;

	UPROPERTY()
	UMyGameInstance* GameInstance;

	UPROPERTY()
	UMissionClearOverlay* MissionClearOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UMissionClearOverlay> MissionClearOverlayClass;

	UPROPERTY()
	UDieMenuOverlay* DieMenuOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UDieMenuOverlay> DieMenuOverlayClass;

	UPROPERTY()
	UMissionPreviewOverlay* MissionPreviewOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UMissionPreviewOverlay> MissionPreviewOverlayClass;

	UPROPERTY()
	UCombatOverlay* CombatOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<UCombatOverlay> CombatOverlayClass;

	UPROPERTY()
	USettingsOverlay* SettingsOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
	TSubclassOf<USettingsOverlay> SettingsOverlayClass;
	
};
