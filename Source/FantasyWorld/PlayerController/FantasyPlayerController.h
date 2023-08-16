// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FantasyPlayerController.generated.h"

class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AFantasyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public :

	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION()
	void RePossess();

	UFUNCTION()
	APawn* GetPawn();

	UFUNCTION()
	void SetMouseSensitivity(float Sensitivity);

	FORCEINLINE float GetMouseSensitivity() const { return MouseSensitivity; }

protected:


private:

	UPROPERTY()
	APawn* MyPawn;

	UPROPERTY(EditAnywhere)
	float MouseSensitivity = 1.f;

	UPROPERTY()
	UMyGameInstance* GameInstance;
};
