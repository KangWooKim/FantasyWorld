// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "SelectCharacterOverlay.generated.h"

class UButton;
class UFileMediaSource;
class USoundBase;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API USelectCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct();

	UFUNCTION()
	void OnCountessButtonClicked();

	UFUNCTION()
	void OnPhaseButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void OnHoveredCountessButton();

	UFUNCTION()
	void OnUnHoveredCountessButton();

	UFUNCTION()
	void OnHoverPhaseButton();

	UFUNCTION()
	void OnUnHoverPhaseButton();

protected:


private:

	UPROPERTY(meta = (BindWidget))
	UButton* CountessButton;

	UPROPERTY(meta = (BindWidget))
	UButton* PhaseButton;

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
