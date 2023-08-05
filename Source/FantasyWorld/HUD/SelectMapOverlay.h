// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapOverlay.generated.h"

class UButton;
class ALobbyLevelHUD;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API USelectMapOverlay : public UUserWidget
{
	GENERATED_BODY()
	

public:

	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnGrassLandButtonClicked();


	UFUNCTION()
		void OnIceLandButtonClicked();


	UFUNCTION()
		void OnInfinityModeButtonClicked();

	UFUNCTION()
		void OnQuitButtonClicked();

protected:


private:

	UPROPERTY(meta = (BindWidget))
	UButton* GrassLandButton;

	UPROPERTY(meta = (BindWidget))
	UButton* IceLandButton;

	UPROPERTY(meta = (BindWidget))
	UButton* InfinityModeButton;

	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;

	UPROPERTY(EditAnywhere)
	USoundBase* ButtonClickedSound;

	UPROPERTY()
	FString MapName;

	UPROPERTY()
	ALobbyLevelHUD* HUD;

};
