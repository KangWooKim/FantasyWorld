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
	// NativeConstruct�� UI ������ ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	void NativeConstruct();

	// QuitButton�� Ŭ���Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
	UFUNCTION()
		void OnQuitButtonClicked();

	// SlayedEnemy �Ķ���͸� ����� �ؽ�Ʈ ����� ������Ʈ�ϴ� �Լ��Դϴ�.
	// int32 SlayedEnemy: óġ�� ���� ���Դϴ�.
	UFUNCTION()
		void UpdateTextBlock(int32 SlayedEnemy);

protected:

	
private:
	// QuitButton�� ���� �������Դϴ�. UButton Ŭ������ �ν��Ͻ��� ����ŵ�ϴ�.
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	// ��ư�� Ŭ������ �� ����� �����Դϴ�.
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// �ؽ�Ʈ ��Ͽ� ���� �������Դϴ�. UTextBlock Ŭ������ �ν��Ͻ��� ����ŵ�ϴ�.
	UPROPERTY(meta = (BindWidget))
		UTextBlock* Text;

};
