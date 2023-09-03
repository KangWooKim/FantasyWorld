// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/HUD/NormalLevelHUD.h"

// NativeConstruct �Լ��� UUserWidget�� �Լ��� �������̵��� ���Դϴ�. UUserWidget�� ������ �� ȣ��˴ϴ�.
void UCombatOverlay::NativeConstruct()
{
    // SoulText�� ��ȿ�ϴٸ� �ʱ� ������ 0�� �����մϴ�.
    if (SoulText) {
        FString CombinedString = FString::Printf(TEXT("%d"), 0); // ���� 0�� ���ڿ��� ��ȯ
        FText NewText = FText::FromString(CombinedString); // ��ȯ�� ���ڿ��� FText�� ��ȯ
        SoulText->SetText(NewText); // SoulText�� ���ο� �ؽ�Ʈ�� ����
    }
}

// HealthBar�� �ۼ�Ƽ���� ������Ʈ�ϴ� �Լ�
void UCombatOverlay::UpdateHealthBar(float Percent)
{
    HealthBar->SetPercent(Percent); // HealthBar�� �ۼ�Ʈ�� ����
}

// StaminaBar�� �ۼ�Ƽ���� ������Ʈ�ϴ� �Լ�
void UCombatOverlay::UpdateStaminaBar(float Percent)
{
    StaminaBar->SetPercent(Percent); // StaminaBar�� �ۼ�Ʈ�� ����
}

// Soul �ؽ�Ʈ�� ������Ʈ�ϴ� �Լ�
void UCombatOverlay::UpdateSoulText(int32 Soul)
{
    // SoulText�� ��ȿ�ϴٸ�
    if (SoulText) {
        FString CombinedString = FString::Printf(TEXT("%d"), Soul); // Soul ���� ���ڿ��� ��ȯ
        FText NewText = FText::FromString(CombinedString); // ��ȯ�� ���ڿ��� FText�� ��ȯ
        SoulText->SetText(NewText); // SoulText�� ���ο� �ؽ�Ʈ�� ����
    }
}

// ���� ���� ���� ǥ���ϴ� �ؽ�Ʈ�� ������Ʈ�ϴ� �Լ�
void UCombatOverlay::UpdateLeftEnemyText(int32 Enemy)
{
    // LeftEnemyText�� ��ȿ�ϴٸ�
    if (LeftEnemyText) {
        FString CombinedString = FString::Printf(TEXT("LeftEnemy : %d"), Enemy); // ���� ���� ���� ���ڿ��� ��ȯ
        FText NewText = FText::FromString(CombinedString); // ��ȯ�� ���ڿ��� FText�� ��ȯ
        LeftEnemyText->SetText(NewText); // LeftEnemyText�� ���ο� �ؽ�Ʈ�� ����
    }
    else return; // LeftEnemyText�� ��ȿ���� �ʴٸ� �Լ��� ����
}
