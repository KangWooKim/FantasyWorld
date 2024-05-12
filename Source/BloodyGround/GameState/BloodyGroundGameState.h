// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "BloodyGroundGameState.generated.h"

UENUM(BlueprintType)
enum class EDayNightCycle : uint8
{
	Day UMETA(DisplayName = "Day"),
	Night UMETA(DisplayName = "Night")
};

UCLASS()
class BLOODYGROUND_API ABloodyGroundGameState : public AGameState
{
	GENERATED_BODY()
	
public:

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    // �������Ʈ���� ���� �� ���� �����ϵ��� ����
    UPROPERTY(ReplicatedUsing = OnRep_DayNightChanged, BlueprintReadWrite, EditDefaultsOnly, Category = "DayNight")
        EDayNightCycle DayNightCycle;

    // ��/�� ���� ����� ȣ��Ǵ� �Լ�
    UFUNCTION()
        void OnRep_DayNightChanged();

    // ��/�� ���¸� �����ϴ� �Լ�
    void SetDayNightCycle(EDayNightCycle NewCycle);
};
