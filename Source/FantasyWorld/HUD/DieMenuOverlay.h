// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DieMenuOverlay.generated.h"

class USoundBase;
class UTextBlock;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UDieMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
	
		// �ۺ� ���� ����
public:

	// NativeConstruct �Լ� �������̵�. UI�� ������ �� �ʱ� ������ �ϱ� ���� �޼���.
	void NativeConstruct() override;

	// ���� �޴��� ���ư��� ��ư�� Ŭ������ �� ����� �Լ��� �����ϴ� UFUNCTION ��ũ��
	UFUNCTION()
		void OnReturnToMenuButtonClicked();

	// ������ ������ϴ� ��ư�� Ŭ������ �� ����� �Լ��� �����ϴ� UFUNCTION ��ũ��
	UFUNCTION()
		void OnRestartLevelButtonClicked();

	// ���� ���� ���� ���� ���� ���� ȭ�鿡 ǥ���ϴ� �Լ�
	// SlayedEnemy: ���� ���� ��, LeftEnemy: ���� ���� ��
	UFUNCTION()
		void UpdateTextBlock(int32 SlayedEnemy, int32 LeftEnemy);


	// �����̺� ���� ����
private:

	// ���� �޴��� ���ư��� ��ư ����. UPROPERTY ��ũ�η� Unreal Engine���� �����ǰ� ����.
	UPROPERTY(meta = (BindWidget))
		class UButton* ReturnToMenu;

	// ������ ������ϴ� ��ư ����. UPROPERTY ��ũ�η� Unreal Engine���� �����ǰ� ����.
	UPROPERTY(meta = (BindWidget))
		UButton* RestartLevel;

	// ��ư�� Ŭ������ �� ����� �Ҹ�. UPROPERTY ��ũ�η� Unreal Engine���� �����ǰ� ����.
	UPROPERTY()
		USoundBase* ButtonClickedSound;

	// ȭ�鿡 ǥ�õ� �ؽ�Ʈ ���. ���� ���� ���� ���� ���� ���� ǥ���� ����.
	// UPROPERTY ��ũ�η� Unreal Engine���� �����ǰ� ����.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text;

};
