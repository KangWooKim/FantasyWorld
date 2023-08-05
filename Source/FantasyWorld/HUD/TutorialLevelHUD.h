// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TutorialLevelHUD.generated.h"

class UTutorialLevelOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATutorialLevelHUD : public AHUD
{
	GENERATED_BODY()

public :

	ATutorialLevelHUD();

	virtual void BeginPlay() override;
	FORCEINLINE UTutorialLevelOverlay* GetTutorialLevelOverlay() { return TutorialLevelOverlay; }

protected:

private:

	UWorld* World;

	APlayerController* PlayerController;

	UPROPERTY(EditDefaultsOnly, Category = "Media")
	TSubclassOf<UTutorialLevelOverlay> TutorialLevelOverlayClass;

	UPROPERTY()
	UTutorialLevelOverlay* TutorialLevelOverlay;
};
