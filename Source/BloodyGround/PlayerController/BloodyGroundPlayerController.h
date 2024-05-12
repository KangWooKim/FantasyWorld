// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BloodyGroundPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLOODYGROUND_API ABloodyGroundPlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    ABloodyGroundPlayerController();

    FORCEINLINE float GetRoundTripTime() { return RoundTripTime; }

protected:
    virtual void PlayerTick(float DeltaTime) override;

private:
    // RTT ���� ����
    UPROPERTY(VisibleAnywhere, Category = "Network")
        float RoundTripTime;

    // ���������� RTT�� ������ ����
    float LastRTTUpdateTime;
};