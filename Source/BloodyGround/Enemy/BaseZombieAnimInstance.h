#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseZombie.h"
#include "BaseZombieAnimInstance.generated.h"

UCLASS()
class BLOODYGROUND_API UBaseZombieAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    // �ִϸ��̼� ������Ʈ �� ȣ��Ǵ� �Լ�
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
    // ������ �̵� �ӵ�
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
        float MovementSpeed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
     EZombieState ZombieState;
};
