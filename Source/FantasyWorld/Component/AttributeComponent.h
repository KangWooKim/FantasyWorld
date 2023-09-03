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
    // �� ������Ʈ�� ������Ƽ�� ���� �⺻ ���� �����մϴ�.
    UAttributeComponent();

protected:
    // ���� ���� �� ȣ��˴ϴ�.
    virtual void BeginPlay() override;

public:
    // �� �����Ӹ��� ȣ��˴ϴ�.
    // DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
    // TickType: �������� ����(�پ��� ƽ ������ ���� �ٸ��� ó���� �� ����)
    // ThisTickFunction: �� ƽ���� � �Լ��� ȣ��Ǿ������� ���� ����
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    // �������� ó���մϴ�.
    // Damage: �޴� ������ ��
    void ReceiveDamage(float Damage);

    // ���׹̳��� ����մϴ�.
    // StaminaCost: ���Ǵ� ���׹̳� ��
    void UseStamina(float StaminaCost);

    // ���� ü���� ü�� �ִ�ġ�� ���� ������� ��ȯ�մϴ�.
    float GetHealthPercent();

    // ���� ���׹̳��� ���׹̳� �ִ�ġ�� ���� ������� ��ȯ�մϴ�.
    float GetStaminaPercent();

    // ĳ���Ͱ� ����ִ��� Ȯ���մϴ�.
    bool IsAlive();

    // ��ȥ�� �߰��մϴ�.
    // NumberOfSouls: �߰��Ǵ� ��ȥ�� ��
    void AddSouls(int32 NumberOfSouls);

    // ��带 �߰��մϴ�.
    // AmountOfGold: �߰��Ǵ� ����� ��
    void AddGold(int AmountOfGold);

    // ���׹̳��� ȸ���մϴ�.
    // DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
    void RegenStamina(float DeltaTime);

    // ��� ���� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
    FORCEINLINE int32 GetGold() const { return Gold; }

    // ��ȥ�� ���� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
    FORCEINLINE int32 GetSouls() const { return Souls; }

    // ȸ���ϴµ� �ʿ��� ���׹̳� ����� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
    FORCEINLINE float GetDodgeCost() const { return DodgeCost; }

    // �����ϴµ� �ʿ��� ���׹̳� ����� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
    FORCEINLINE float GetAttackCost() const { return AttackCost; }

    // ���� ���׹̳� ���� ��ȯ�ϴ� �ζ��� �Լ��Դϴ�.
    FORCEINLINE float GetStamina() const { return Stamina; }

private:

    // ���� ü��
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float Health;

    // �ִ� ü��
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float MaxHealth;

    // ���� ���׹̳�
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float Stamina;

    // �ִ� ���׹̳�
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float MaxStamina;

    // ���� ���
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        int32 Gold;

    // ���� ��ȥ
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        int32 Souls;

    // ���ݿ� �ʿ��� ���׹̳� ���
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float AttackCost = 15.f;

    // ȸ�ǿ� �ʿ��� ���׹̳� ���
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float DodgeCost = 14.f;

    // ���׹̳� ȸ����
    UPROPERTY(EditAnywhere, Category = "Actor Attributes")
        float StaminaRegenRate = 8.f;

};
