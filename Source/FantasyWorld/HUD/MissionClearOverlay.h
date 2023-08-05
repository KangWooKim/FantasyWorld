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

	void NativeConstruct();

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void UpdateTextBlock(int32 SlayedEnemy);

protected:


private:

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere)
	USoundBase* ButtonClickedSound;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text;
};
