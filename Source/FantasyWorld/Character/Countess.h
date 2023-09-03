// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "CharacterTypes.h"
#include "Countess.generated.h"

class UBoxComponent;
class USoundBase;
class UAnimMontage;
class UParticleSystem;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ACountess : public ABaseCharacter
{
	GENERATED_BODY()
	
        // ACountess Ŭ������ ��� ���� �κ��Դϴ�.

public:
    // �⺻ ������
    ACountess();

    // �Է� ������Ʈ ������ ���� ���� �Լ��Դϴ�. UInputComponent Ÿ���� PlayerInputComponent�� ���ڷ� �޽��ϴ�.
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // ���� ���� �� �� ���� ȣ��Ǵ� ���� �Լ��Դϴ�.
    virtual void BeginPlay() override;

    // �����Ӹ��� ȣ��Ǵ� ���� �Լ��Դϴ�. DeltaTime�� ���� �����Ӱ� ���� ������ ������ �ð��� �� ������ ��Ÿ���ϴ�.
    virtual void Tick(float DeltaTime) override;

    // ������ �����ϴ� ���� �Լ��Դϴ�.
    virtual void Attack() override;

    // �ʻ� ��带 Ȱ��ȭ/��Ȱ��ȭ�ϴ� ���� �Լ��Դϴ�.
    virtual void LethalMode() override;

    // �ʻ� ��带 �����ϴ� ���� �Լ��Դϴ�.
    virtual void LethalModeFinish() override;

private:
    // �ʻ�⸦ ����� ���� �Һ� �����Դϴ�. EditAnywhere ��Ʈ����Ʈ�� �����Ϳ��� ���� �����մϴ�.
    UPROPERTY(EditAnywhere)
        int32 ConsumptionRate = 1;

    // ���������� �ҿ� Ȥ�� ��Ÿ �ڿ��� �Һ��� ���� ����� �ð��Դϴ�.
    UPROPERTY()
        float TimeSinceLastDecrease = 0.0f;

    // �Ϲ� ������ �������Դϴ�. EditAnywhere ��Ʈ����Ʈ�� �����Ϳ��� ���� �����մϴ�.
    UPROPERTY(EditAnywhere)
        float NormalDamage = 20.f;

    // �ʻ�⸦ ����� ���� �������Դϴ�. EditAnywhere ��Ʈ����Ʈ�� �����Ϳ��� ���� �����մϴ�.
    UPROPERTY(EditAnywhere)
        float ELSDamage = 100.f;

    // ������ �� �� ��µǴ� ���� ȿ���Դϴ�. EditAnywhere ��Ʈ����Ʈ�� �����Ϳ��� ���� �����մϴ�.
    UPROPERTY(EditAnywhere, Category = "Combat")
        UParticleSystem* SwingEffect;

    // �ʻ�⸦ ����� �� �Ҹ�Ǵ� ü���Դϴ�. EditAnywhere ��Ʈ����Ʈ�� �����Ϳ��� ���� �����մϴ�.
    UPROPERTY(EditAnywhere)
        float SpendHealthAmount = 10.f;

};
