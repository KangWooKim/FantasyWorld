// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatOverlay.generated.h"

class UButton;
class USoundBase;
class UTextBlock;
class UProgressBar;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UCombatOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// �⺻ �������Դϴ�.
	void NativeConstruct();

	// ü�� ���� �ۼ�Ƽ���� ������Ʈ�ϴ� �Լ��Դϴ�.
	// @Percent: ������Ʈ �� ü���� ���� (0.0 ~ 1.0)
	UFUNCTION()
		void UpdateHealthBar(float Percent);

	// ���¹̳� ���� �ۼ�Ƽ���� ������Ʈ�ϴ� �Լ��Դϴ�.
	// @Percent: ������Ʈ �� ���¹̳��� ���� (0.0 ~ 1.0)
	UFUNCTION()
		void UpdateStaminaBar(float Percent);

	// �ҿ� �ؽ�Ʈ�� ������Ʈ�ϴ� �Լ��Դϴ�.
	// @Soul: ǥ���� �ҿ��� ��
	UFUNCTION()
		void UpdateSoulText(int32 Soul);

	// ���� ���� ���� ǥ���ϴ� �ؽ�Ʈ�� ������Ʈ�ϴ� �Լ��Դϴ�.
	// @Enemy: ���� ���� ��
	UFUNCTION()
		void UpdateLeftEnemyText(int32 Enemy);

protected:

	// ����� protected �����Դϴ�. �ش� Ŭ������ �ڽ� Ŭ���������� ���� �����մϴ�.

private:

	// ��ư Ŭ�� ���忡 ���� ���带 �����մϴ�.
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// �ҿ��� ���� ǥ���� �ؽ�Ʈ ����� �����մϴ�.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* SoulText;

	// ���� ���� ���� ǥ���� �ؽ�Ʈ ����� �����մϴ�.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* LeftEnemyText;

	// ü�� �ٸ� �����մϴ�.
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

	// ���¹̳� �ٸ� �����մϴ�.
	UPROPERTY(meta = (BindWidget))
		UProgressBar* StaminaBar;

};
