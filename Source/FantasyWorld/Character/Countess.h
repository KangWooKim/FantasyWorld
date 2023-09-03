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
	
        // ACountess 클래스의 헤더 선언 부분입니다.

public:
    // 기본 생성자
    ACountess();

    // 입력 컴포넌트 설정을 위한 가상 함수입니다. UInputComponent 타입의 PlayerInputComponent를 인자로 받습니다.
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    // 게임 시작 시 한 번만 호출되는 가상 함수입니다.
    virtual void BeginPlay() override;

    // 프레임마다 호출되는 가상 함수입니다. DeltaTime은 이전 프레임과 현재 프레임 사이의 시간을 초 단위로 나타냅니다.
    virtual void Tick(float DeltaTime) override;

    // 공격을 수행하는 가상 함수입니다.
    virtual void Attack() override;

    // 필살 모드를 활성화/비활성화하는 가상 함수입니다.
    virtual void LethalMode() override;

    // 필살 모드를 종료하는 가상 함수입니다.
    virtual void LethalModeFinish() override;

private:
    // 필살기를 사용할 때의 소비 비율입니다. EditAnywhere 어트리뷰트로 에디터에서 수정 가능합니다.
    UPROPERTY(EditAnywhere)
        int32 ConsumptionRate = 1;

    // 마지막으로 소울 혹은 기타 자원을 소비한 이후 경과한 시간입니다.
    UPROPERTY()
        float TimeSinceLastDecrease = 0.0f;

    // 일반 공격의 데미지입니다. EditAnywhere 어트리뷰트로 에디터에서 수정 가능합니다.
    UPROPERTY(EditAnywhere)
        float NormalDamage = 20.f;

    // 필살기를 사용할 때의 데미지입니다. EditAnywhere 어트리뷰트로 에디터에서 수정 가능합니다.
    UPROPERTY(EditAnywhere)
        float ELSDamage = 100.f;

    // 공격을 할 때 출력되는 입자 효과입니다. EditAnywhere 어트리뷰트로 에디터에서 수정 가능합니다.
    UPROPERTY(EditAnywhere, Category = "Combat")
        UParticleSystem* SwingEffect;

    // 필살기를 사용할 때 소모되는 체력입니다. EditAnywhere 어트리뷰트로 에디터에서 수정 가능합니다.
    UPROPERTY(EditAnywhere)
        float SpendHealthAmount = 10.f;

};
