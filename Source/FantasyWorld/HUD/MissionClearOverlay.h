// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionClearOverlay.generated.h"

class UButton;
class USoundBase;
class UTextBlock;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UMissionClearOverlay : public UUserWidget
{
	GENERATED_BODY()
	
		
public:
	// NativeConstruct는 UI 위젯이 생성될 때 호출되는 함수입니다.
	void NativeConstruct();

	// QuitButton이 클릭되었을 때 호출되는 함수입니다.
	UFUNCTION()
		void OnQuitButtonClicked();

	// SlayedEnemy 파라미터를 사용해 텍스트 블록을 업데이트하는 함수입니다.
	// int32 SlayedEnemy: 처치한 적의 수입니다.
	UFUNCTION()
		void UpdateTextBlock(int32 SlayedEnemy);

protected:

	
private:
	// QuitButton에 대한 포인터입니다. UButton 클래스의 인스턴스를 가리킵니다.
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	// 버튼이 클릭됐을 때 재생될 사운드입니다.
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// 텍스트 블록에 대한 포인터입니다. UTextBlock 클래스의 인스턴스를 가리킵니다.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text;

};
