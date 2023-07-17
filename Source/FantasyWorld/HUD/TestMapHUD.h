// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/HUD.h"
#include "TestMapHUD.generated.h"

class UDieMenuOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATestMapHUD : public AHUD
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditDefaultsOnly, Category = Fantasy)
	TSubclassOf<UDieMenuOverlay> DieMenuOverlayClass;

	UPROPERTY()
	UDieMenuOverlay* DieMenuOverlay;

public:
	
};
