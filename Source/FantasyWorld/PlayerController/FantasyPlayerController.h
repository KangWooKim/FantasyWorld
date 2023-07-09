// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FantasyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AFantasyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :

	virtual void BeginPlay() override;

protected:


private:



};
