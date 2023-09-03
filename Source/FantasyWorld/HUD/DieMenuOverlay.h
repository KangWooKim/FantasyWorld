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
	
		// 퍼블릭 섹션 시작
public:

	// NativeConstruct 함수 오버라이딩. UI가 생성될 때 초기 설정을 하기 위한 메서드.
	void NativeConstruct() override;

	// 메인 메뉴로 돌아가는 버튼을 클릭했을 때 실행될 함수를 지정하는 UFUNCTION 매크로
	UFUNCTION()
		void OnReturnToMenuButtonClicked();

	// 레벨을 재시작하는 버튼을 클릭했을 때 실행될 함수를 지정하는 UFUNCTION 매크로
	UFUNCTION()
		void OnRestartLevelButtonClicked();

	// 죽은 적의 수와 남은 적의 수를 화면에 표시하는 함수
	// SlayedEnemy: 죽은 적의 수, LeftEnemy: 남은 적의 수
	UFUNCTION()
		void UpdateTextBlock(int32 SlayedEnemy, int32 LeftEnemy);


	// 프라이빗 섹션 시작
private:

	// 메인 메뉴로 돌아가는 버튼 위젯. UPROPERTY 매크로로 Unreal Engine에서 관리되게 설정.
	UPROPERTY(meta = (BindWidget))
		class UButton* ReturnToMenu;

	// 레벨을 재시작하는 버튼 위젯. UPROPERTY 매크로로 Unreal Engine에서 관리되게 설정.
	UPROPERTY(meta = (BindWidget))
		UButton* RestartLevel;

	// 버튼을 클릭했을 때 재생될 소리. UPROPERTY 매크로로 Unreal Engine에서 관리되게 설정.
	UPROPERTY()
		USoundBase* ButtonClickedSound;

	// 화면에 표시될 텍스트 블록. 죽은 적의 수와 남은 적의 수를 표시할 예정.
	// UPROPERTY 매크로로 Unreal Engine에서 관리되게 설정.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text;

};
