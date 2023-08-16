// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/HUD/NormalLevelHUD.h"

void UCombatOverlay::NativeConstruct()
{
	if (SoulText) {
		FString CombinedString = FString::Printf(TEXT("%d"), 0);
		FText NewText = FText::FromString(CombinedString);
		SoulText->SetText(NewText);
	}
}

void UCombatOverlay::UpdateHealthBar(float Percent)
{
	HealthBar->SetPercent(Percent);
}

void UCombatOverlay::UpdateStaminaBar(float Percent)
{
	StaminaBar->SetPercent(Percent);
}

void UCombatOverlay::UpdateSoulText(int32 Soul)
{
	if (SoulText) {
		FString CombinedString = FString::Printf(TEXT("%d"), Soul);
		FText NewText = FText::FromString(CombinedString);
		SoulText->SetText(NewText);
	}
}

void UCombatOverlay::UpdateLeftEnemyText(int32 Enemy)
{
	if (LeftEnemyText) {
		FString CombinedString = FString::Printf(TEXT("LeftEnemy : %d"), Enemy);
		FText NewText = FText::FromString(CombinedString);
		LeftEnemyText->SetText(NewText);
	}
	else return;
}



