// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DieMenuOverlay.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UDieMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public :

	void NativeConstruct();
	void OnReturnToMenuButtonClicked();
	void OnRestartLevelButtonClicked();


private:

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnToMenu;

	UPROPERTY(meta = (BindWidget))
	UButton* RestartLevel;
};
