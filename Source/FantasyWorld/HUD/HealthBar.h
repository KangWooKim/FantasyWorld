// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
public :

	// 체력 바를 참조합니다.
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

};
