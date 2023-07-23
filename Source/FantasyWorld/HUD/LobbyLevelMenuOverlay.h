// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyLevelMenuOverlay.generated.h"

class UButton;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ULobbyLevelMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct();
	void OnStartGameButtonMenuButtonClicked();
	void OnTutorialButtonClicked();
	void OnSettingsButtonClicked();
	void OnQuitButtonClicked();

protected:

private:

	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* TutorialButton;

	UPROPERTY(meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
};
