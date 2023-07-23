// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "BaseCharacter.h"

void UBaseCharacterAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();

    // 소유자를 CountessCharacter 형식으로 캐스팅
    BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
    if (BaseCharacter) {
        // 캐릭터의 이동 정보를 가져옵니다.
        CharacterMovement = BaseCharacter->GetCharacterMovement();
    }
}

// 애니메이션 업데이트 함수
// @param DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (CharacterMovement) {
        // 캐릭터의 이동 속도를 계산합니다. 
        GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);

        // 캐릭터가 공중에 있는지 확인합니다.
        IsFalling = CharacterMovement->IsFalling();



        // 캐릭터의 행동 상태를 가져옵니다.
        ActionState = BaseCharacter->GetActionState();

        // 캐릭터의 죽음 포즈를 가져옵니다.
        DeathPose = BaseCharacter->GetDeathPose();
    }
}