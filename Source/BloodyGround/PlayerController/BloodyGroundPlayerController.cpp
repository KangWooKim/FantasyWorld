// Fill out your copyright notice in the Description page of Project Settings.


#include "BloodyGroundPlayerController.h"
#include "GameFramework/PlayerState.h"


ABloodyGroundPlayerController::ABloodyGroundPlayerController()
{
    // RTT �ʱ�ȭ
    RoundTripTime = 0.0f;
    LastRTTUpdateTime = 0.0f;
}

void ABloodyGroundPlayerController::PlayerTick(float DeltaTime)
{
    Super::PlayerTick(DeltaTime);

    // RTT ����
    if (GetWorld()->TimeSince(LastRTTUpdateTime) > 1.0f)  // ��: �� 1�ʸ��� ����
    {
        if (PlayerState && IsLocalController())
        {
            RoundTripTime = PlayerState->GetPing() * 0.001f;  // �и��ʸ� �ʷ� ��ȯ
            LastRTTUpdateTime = GetWorld()->GetTimeSeconds();
        }
    }
}