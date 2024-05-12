// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodyGroundGameState.h"
#include "Net/UnrealNetwork.h"

void ABloodyGroundGameState::SetDayNightCycle(EDayNightCycle NewCycle)
{
    if (HasAuthority())
    {
        DayNightCycle = NewCycle;
    }
}

void ABloodyGroundGameState::OnRep_DayNightChanged()
{
    // ��/�� ���°� ����Ǿ��� �� �ʿ��� ������ ���⿡ �����մϴ�.
    // ��: ���� �㿡 ���� ȯ�� ���� ����, ���� AI ���� ���� ��
}

void ABloodyGroundGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // ��/�� ���� ���� ����
    DOREPLIFETIME(ABloodyGroundGameState, DayNightCycle);
}