// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "LobbyLevelMenuOverlay.generated.h"

class UButton;
class USoundBase;
class UFileMediaSource;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ULobbyLevelMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct();

	UFUNCTION()
	void OnStartGameButtonMenuButtonClicked();

	UFUNCTION()
	void OnTutorialButtonClicked();

	UFUNCTION()
	void OnSettingsButtonClicked();

	UFUNCTION()
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

	UPROPERTY(EditAnywhere)
	USoundBase* ButtonClickedSound;

	UPROPERTY(EditAnywhere)
	UMediaPlayer* MediaPlayer;

	UPROPERTY(EditAnywhere)
	TArray<UFileMediaSource*> MediaSources;

	UPROPERTY(EditAnywhere)
	UMediaTexture* MediaTexture;

	UPROPERTY()
	UTexture2D* Texture;

	UPROPERTY()
	FSlateBrush Brush;

	UPROPERTY(meta = (BindWidget))
	UImage* Image;
};
