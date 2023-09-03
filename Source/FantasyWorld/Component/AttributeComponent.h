// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FANTASYWORLD_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
    // 이 컴포넌트의 프로퍼티에 대한 기본 값을 설정합니다.
    UAttributeComponent();

protected:
    // 게임 시작 시 호출됩니다.
    virtual void BeginPlay() override;

public:
    // 매 프레임마다 호출됩니다.
    // DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
    // TickType: 프레임의 유형(다양한 틱 유형에 따라 다르게 처리할 수 있음)
    // ThisTickFunction: 이 틱에서 어떤 함수가 호출되었는지에 대한 정보
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // 데미지를 처리합니다.
    // Damage: 받는 데미지 양
    void ReceiveDamage(float Damage);

    // 스테미나를 사용합니다.
    // StaminaCost: 사용되는 스테미나 양
    void UseStamina(float StaminaCost);

    // 현재 체력을 체력 최대치로 나눈 백분율을 반환합니다.
    float GetHealthPercent();

    // 현재 스테미나를 스테미나 최대치로 나눈 백분율을 반환합니다.
    float GetStaminaPercent();

    // 캐릭터가 살아있는지 확인합니다.
    bool IsAlive();

    // 영혼을 추가합니다.
    // NumberOfSouls: 추가되는 영혼의 수
    void AddSouls(int32 NumberOfSouls);

    // 골드를 추가합니다.
    // AmountOfGold: 추가되는 골드의 양
    void AddGold(int AmountOfGold);

    // 스테미나를 회복합니다.
    // DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
    void RegenStamina(float DeltaTime);

    // 골드 양을 반환하는 인라인 함수입니다.
    FORCEINLINE int32 GetGold() const { return Gold; }

    // 영혼의 수를 반환하는 인라인 함수입니다.
    FORCEINLINE int32 GetSouls() const { return Souls; }

    // 회피하는데 필요한 스테미나 비용을 반환하는 인라인 함수입니다.
    FORCEINLINE float GetDodgeCost() const { return DodgeCost; }

    // 공격하는데 필요한 스테미나 비용을 반환하는 인라인 함수입니다.
    FORCEINLINE float GetAttackCost() const { return AttackCost; }

    // 현재 스테미나 양을 반환하는 인라인 함수입니다.
    FORCEINLINE float GetStamina() const { return Stamina; }

private:

    // 현재 체력
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float Health;

    // 최대 체력
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float MaxHealth;

    // 현재 스테미나
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float Stamina;

    // 최대 스테미나
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float MaxStamina;

    // 현재 골드
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        int32 Gold;

    // 현재 영혼
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        int32 Souls;

    // 공격에 필요한 스테미나 비용
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float AttackCost = 15.f;

    // 회피에 필요한 스테미나 비용
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float DodgeCost = 14.f;

    // 스테미나 회복률
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float StaminaRegenRate = 8.f;

};
