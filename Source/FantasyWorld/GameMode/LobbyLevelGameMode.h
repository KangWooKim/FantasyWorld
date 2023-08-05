// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyLevelGameMode.generated.h"

class USoundBase;
class UFileMediaSource;
class UMediaPlayer;
class UAudioComponent;
class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ALobbyLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

	ALobbyLevelGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

private:

	UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
	USoundBase* BackGroundMusic;

	UPROPERTY()
	UAudioComponent* BGMComponent;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	UMyGameInstance* GameInstance;
};
