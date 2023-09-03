// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FantasyPlayerController.generated.h"

class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AFantasyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

    // BeginPlay()는 게임이 시작되면 한 번만 호출되는 함수
    // 이 함수를 오버라이드해서 플레이어가 게임에 참여했을 때 초기 설정을 할 수 있다.
    virtual void BeginPlay() override;

    // PlayerTick()는 프레임마다 호출되는 함수
    // DeltaTime 인자는 이전 프레임과 현재 프레임 사이의 시간을 초 단위로 나타낸다.
    virtual void PlayerTick(float DeltaTime) override;

    // RePossess()는 캐릭터의 소유권을 다시 얻을 때 호출하는 함수
    // 예를 들어, 캐릭터가 죽었다가 부활했을 때 이 함수를 사용할 수 있다.
    UFUNCTION()
        void RePossess();

    // GetPawn()은 현재 플레이어 컨트롤러가 조종하고 있는 폰(캐릭터)을 반환하는 함수
    UFUNCTION()
        APawn* GetPawn();

    // SetMouseSensitivity()는 마우스의 민감도를 설정하는 함수
    // Sensitivity 인자는 새로 설정할 마우스 민감도 값
    UFUNCTION()
        void SetMouseSensitivity(float Sensitivity);

    // GetMouseSensitivity()는 현재 설정된 마우스 민감도를 반환하는 함수
    // FORCEINLINE은 컴파일러에 이 함수를 인라인으로 만들라고 지시하는 키워드
    FORCEINLINE float GetMouseSensitivity() const { return MouseSensitivity; }

protected:
    

private:

    // MyPawn은 현재 플레이어 컨트롤러가 조종하고 있는 폰(캐릭터)의 참조
    UPROPERTY()
        APawn* MyPawn;

    // MouseSensitivity는 마우스의 민감도를 저장하는 변수, 초기값은 1.f
    UPROPERTY(EditAnywhere)
        float MouseSensitivity = 1.f;

    // GameInstance는 현재 게임 인스턴스의 참조
    // UMyGameInstance 타입의 객체에 접근할 수 있어 게임 전반의 설정이나 상태를 관리할 수 있다.
    UPROPERTY()
        UMyGameInstance* GameInstance;

};
