// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/HUD/NormalLevelHUD.h"

// NativeConstruct 함수는 UUserWidget의 함수를 오버라이드한 것입니다. UUserWidget이 생성될 때 호출됩니다.
void UCombatOverlay::NativeConstruct()
{
    // SoulText가 유효하다면 초기 값으로 0을 설정합니다.
    if (SoulText) {
        FString CombinedString = FString::Printf(TEXT("%d"), 0); // 숫자 0을 문자열로 변환
        FText NewText = FText::FromString(CombinedString); // 변환된 문자열을 FText로 변환
        SoulText->SetText(NewText); // SoulText에 새로운 텍스트를 설정
    }
}

// HealthBar의 퍼센티지를 업데이트하는 함수
void UCombatOverlay::UpdateHealthBar(float Percent)
{
    HealthBar->SetPercent(Percent); // HealthBar의 퍼센트를 설정
}

// StaminaBar의 퍼센티지를 업데이트하는 함수
void UCombatOverlay::UpdateStaminaBar(float Percent)
{
    StaminaBar->SetPercent(Percent); // StaminaBar의 퍼센트를 설정
}

// Soul 텍스트를 업데이트하는 함수
void UCombatOverlay::UpdateSoulText(int32 Soul)
{
    // SoulText가 유효하다면
    if (SoulText) {
        FString CombinedString = FString::Printf(TEXT("%d"), Soul); // Soul 값을 문자열로 변환
        FText NewText = FText::FromString(CombinedString); // 변환된 문자열을 FText로 변환
        SoulText->SetText(NewText); // SoulText에 새로운 텍스트를 설정
    }
}

// 남은 적의 수를 표시하는 텍스트를 업데이트하는 함수
void UCombatOverlay::UpdateLeftEnemyText(int32 Enemy)
{
    // LeftEnemyText가 유효하다면
    if (LeftEnemyText) {
        FString CombinedString = FString::Printf(TEXT("LeftEnemy : %d"), Enemy); // 남은 적의 수를 문자열로 변환
        FText NewText = FText::FromString(CombinedString); // 변환된 문자열을 FText로 변환
        LeftEnemyText->SetText(NewText); // LeftEnemyText에 새로운 텍스트를 설정
    }
    else return; // LeftEnemyText가 유효하지 않다면 함수를 종료
}
