// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
#include "Countess.generated.h"

class UBoxComponent;
class USoundBase;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ACountess : public ABaseCharacter
{
	GENERATED_BODY()
	
public : 

	ACountess();

protected : 

	virtual void BeginPlay() override;

private : 

	
};
