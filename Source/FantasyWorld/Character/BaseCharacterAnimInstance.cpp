// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // �����ڸ� CountessCharacter �������� ĳ����
    BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
    if (BaseCharacter) {
        // ĳ������ �̵� ������ �����ɴϴ�.
        CharacterMovement = BaseCharacter->GetCharacterMovement();
    }
}

// �ִϸ��̼� ������Ʈ �Լ�
// @param DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (CharacterMovement) {
        // ĳ������ �̵� �ӵ��� ����մϴ�. 
        GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);

        // ĳ���Ͱ� ���߿� �ִ��� Ȯ���մϴ�.
        IsFalling = CharacterMovement->IsFalling();



        // ĳ������ �ൿ ���¸� �����ɴϴ�.
        ActionState = BaseCharacter->GetActionState();

        // ĳ������ ���� ��� �����ɴϴ�.
        DeathPose = BaseCharacter->GetDeathPose();
    }
}