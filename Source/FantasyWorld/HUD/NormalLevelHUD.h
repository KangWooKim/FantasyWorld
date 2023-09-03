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

	// ANormalLevelHUD�� ������ ����
	ANormalLevelHUD();
	// BeginPlay() �Լ��� �������̵�. ������ ����Ǹ� ó������ ȣ���.
	virtual void BeginPlay() override;

	// �̼� �Ϸ� �������̸� �����ϴ� �Լ�
	UFUNCTION()
		void CreateMissionClearOverlay();

	// �̼� �Ϸ� �������̸� ǥ���ϴ� �Լ�
	// SlayedEnemy: �÷��̾ ���� ���� ��
	UFUNCTION()
		void ShowMissionClearOverlay(int32 SlayedEnemy);

	// �̼� �Ϸ� �������̸� ����� �Լ�
	UFUNCTION()
		void HideMissionClearOverlay();

	// ���� �޴� �������̸� �����ϴ� �Լ�
	UFUNCTION()
		void CreateDieMenuOverlay();

	// ���� �޴� �������̸� ǥ���ϴ� �Լ�
	// SlayedEnemy: �÷��̾ �ױ� ���� ���� ���� ��
	// LeftEnemy: �÷��̾ ���� �� ���� ���� ��
	UFUNCTION()
		void ShowDieMenuOverlay(int32 SlayedEnemy, int32 LeftEnemy);

	// ���� �޴� �������̸� ����� �Լ�
	UFUNCTION()
		void HideDieMenuOverlay();

	// �̼� ������ �������̸� �����ϴ� �Լ�
	UFUNCTION()
		void RemoveMissionPreviewOverlay();

	// ���� �������̸� �����ϴ� �Լ�
	UFUNCTION()
		void CreateCombatOverlay();

	// ���� �������̸� ǥ���ϴ� �Լ�
	UFUNCTION()
		void ShowCombatOverlay();

	// ���� �������̸� ����� �Լ�
	UFUNCTION()
		void HideCombatOverlay();

	// ���� �������̸� �����ϴ� �Լ�
	UFUNCTION()
		void CreateSettingsOverlay();

	// ���� �������̸� ǥ���ϴ� �Լ�
	UFUNCTION()
		void ShowSettingsOverlay();

	// ���� �������̸� ����� �Լ�
	UFUNCTION()
		void HideSettingsOverlay();

	// CombatOverlay�� ��ȯ�ϴ� �ζ��� �Լ�, FORCEINLINE�� �����Ϸ��� �ζ��� Ȯ���� ����
	UFUNCTION()
		FORCEINLINE UCombatOverlay* GetCombatOverlay() { return CombatOverlay; }


protected:


	// Ŭ������ private ���� ����
private:

	// UWorld ��ü�� ���� ������. ���� ������ ���踦 ��Ÿ��.
	UPROPERTY()
		UWorld* World;

	// AFantasyPlayerController�� ���� ������. �÷��̾��� ��Ʈ���� ����.
	UPROPERTY()
		AFantasyPlayerController* PlayerController;

	// UMyGameInstance�� ���� ������. ������ �������� ���³� ������ ����.
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// �̼� �Ϸ� �������̿� ���� ������.
	UPROPERTY()
		UMissionClearOverlay* MissionClearOverlay;

	// �̼� �Ϸ� �������� Ŭ������ UClass Ÿ��.
	// EditDefaultsOnly: �� ������ �����Ϳ����� �⺻���� ������ �� ����.
	// Category = "Overlay": �����Ϳ��� �� ������ "Overlay" ī�װ��� ���ϵ��� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UMissionClearOverlay> MissionClearOverlayClass;

	// ���� �޴� �������̿� ���� ������.
	UPROPERTY()
		UDieMenuOverlay* DieMenuOverlay;

	// ���� �޴� �������� Ŭ������ UClass Ÿ��.
	// EditDefaultsOnly�� Category�� ���� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UDieMenuOverlay> DieMenuOverlayClass;

	// �̼� ������ �������̿� ���� ������.
	UPROPERTY()
		UMissionPreviewOverlay* MissionPreviewOverlay;

	// �̼� ������ �������� Ŭ������ UClass Ÿ��.
	// EditDefaultsOnly�� Category�� ���� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UMissionPreviewOverlay> MissionPreviewOverlayClass;

	// ���� �������̿� ���� ������.
	UPROPERTY()
		UCombatOverlay* CombatOverlay;

	// ���� �������� Ŭ������ UClass Ÿ��.
	// EditDefaultsOnly�� Category�� ���� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<UCombatOverlay> CombatOverlayClass;

	// ���� �������̿� ���� ������.
	UPROPERTY()
		USettingsOverlay* SettingsOverlay;

	// ���� �������� Ŭ������ UClass Ÿ��.
	// EditDefaultsOnly�� Category�� ���� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Overlay")
		TSubclassOf<USettingsOverlay> SettingsOverlayClass;

	
};
