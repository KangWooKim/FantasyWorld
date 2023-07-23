// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyLevelHUD.generated.h"

class ULobbyLevelMenuOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ALobbyLevelHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

protected:

private:

	UWorld* World;
	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Menu")
	TSubclassOf<ULobbyLevelMenuOverlay> LobbyLevelMenuOverlayClass;

	UPROPERTY()
	ULobbyLevelMenuOverlay* LobbyLevelMenuOverlay;
};
