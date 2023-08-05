// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyLevelHUD.generated.h"

class ULobbyLevelMenuOverlay;
class USelectMapOverlay;
class USelectCharacterOverlay;
class USettingsOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ALobbyLevelHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	UFUNCTION()
	void CreateSelectMapOverlay();

	UFUNCTION()
	void ShowSelectMapOverlay();

	UFUNCTION()
	void HideSelectMapOverlay();

	UFUNCTION()
	void CreateSelectCharacterOverlay();

	UFUNCTION()
	void ShowSelectCharacterOverlay();

	UFUNCTION()
	void HideSelectCharacterOverlay();

	UFUNCTION()
	void CreateSettingsOverlay();

	UFUNCTION()
	void ShowSettingsOverlay();

	UFUNCTION()
	void HideSettingsOverlay();

protected:



private:

	UWorld* World;
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<ULobbyLevelMenuOverlay> LobbyLevelMenuOverlayClass;

	UPROPERTY()
	ULobbyLevelMenuOverlay* LobbyLevelMenuOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<USelectMapOverlay> SelectMapOverlayClass;

	UPROPERTY()
	USelectMapOverlay* SelectMapOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<USelectCharacterOverlay> SelectCharacterOverlayClass;

	UPROPERTY()
	USelectCharacterOverlay* SelectCharacterOverlay;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<USettingsOverlay> SettingsOverlayClass;

	UPROPERTY()
	USettingsOverlay* SettingsOverlay;
};
