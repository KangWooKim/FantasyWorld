#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseCharacter.h"
#include "BloodyGround/Weapon/BaseWeapon.h"
#include "BaseCharacterAnimInstance.generated.h"

UCLASS()
class BLOODYGROUND_API UBaseCharacterAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // ĳ������ ���� �ӵ�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
        float GroundSpeed;

    // ĳ������ ���� �̵���
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
        float ForwardMovement;

    // ĳ������ �¿� �̵���
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
        float RightMovement;

    // ĳ���Ͱ� ���߿� �ִ��� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
        bool bIsInAir;

    // ĳ���Ͱ� �����ϰ� �ִ��� ����
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action")
        bool bIsAiming;

    // ���� ĳ���Ͱ� ��� �ִ� ������ Ÿ��
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
        EWeaponType CurrentWeaponType;

    // ���� ���� �ִϸ��̼� ��� �Լ�
    UFUNCTION()
        void PlayWeaponSwitchAnimation();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action")
    FVector LeftHandSocketLocation;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Action")
        ECharacterState CharacterState;

protected:
    // �ִϸ��̼� ������Ʈ �� ȣ��Ǵ� �Լ�
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};