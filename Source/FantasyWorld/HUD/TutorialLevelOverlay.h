// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "TutorialLevelOverlay.generated.h"

class UButton;
class UFileMediaSource;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UTutorialLevelOverlay : public UUserWidget
{
	GENERATED_BODY()

public :

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnOkButtonClicked();

	UFUNCTION()
	void OnQuitButtonClicked();

	UFUNCTION()
	void PlayNextVideo();

protected:

private:

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

	UPROPERTY()
	TArray<FString> VideoUrls;

	UPROPERTY(EditAnywhere)
	int32 CurrentVideoIndex = 0;


	UPROPERTY(meta = (BindWidget))
	UButton* OkButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	
};
