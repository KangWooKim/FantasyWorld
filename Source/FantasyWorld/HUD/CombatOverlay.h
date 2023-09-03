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

	// 기본 생성자입니다.
	void NativeConstruct();

	// 체력 바의 퍼센티지를 업데이트하는 함수입니다.
	// @Percent: 업데이트 할 체력의 비율 (0.0 ~ 1.0)
	UFUNCTION()
		void UpdateHealthBar(float Percent);

	// 스태미나 바의 퍼센티지를 업데이트하는 함수입니다.
	// @Percent: 업데이트 할 스태미나의 비율 (0.0 ~ 1.0)
	UFUNCTION()
		void UpdateStaminaBar(float Percent);

	// 소울 텍스트를 업데이트하는 함수입니다.
	// @Soul: 표시할 소울의 양
	UFUNCTION()
		void UpdateSoulText(int32 Soul);

	// 남은 적의 수를 표시하는 텍스트를 업데이트하는 함수입니다.
	// @Enemy: 남은 적의 수
	UFUNCTION()
		void UpdateLeftEnemyText(int32 Enemy);

protected:

	// 여기는 protected 영역입니다. 해당 클래스와 자식 클래스에서만 접근 가능합니다.

private:

	// 버튼 클릭 사운드에 사용될 사운드를 참조합니다.
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// 소울의 양을 표시할 텍스트 블록을 참조합니다.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* SoulText;

	// 남은 적의 수를 표시할 텍스트 블록을 참조합니다.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* LeftEnemyText;

	// 체력 바를 참조합니다.
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

	// 스태미나 바를 참조합니다.
	UPROPERTY(meta = (BindWidget))
		UProgressBar* StaminaBar;

};
