// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyLevelGameMode.generated.h"

class USoundBase;
class UFileMediaSource;
class UMediaPlayer;
class UAudioComponent;
class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ALobbyLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

    // 기본 생성자
    ALobbyLevelGameMode();

    // BeginPlay 오버라이드: 게임 시작 시 호출되는 함수
    virtual void BeginPlay() override;

    // Tick 오버라이드: 프레임마다 호출되는 함수, DeltaTime은 이전 프레임과의 시간 차이
    virtual void Tick(float DeltaTime) override;

protected:

    // protected 멤버는 여기에 위치함

private:

    // 배경 음악에 사용할 사운드. 에디터에서 설정 가능
    UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
        USoundBase* BackGroundMusic;

    // 배경 음악을 재생하기 위한 오디오 컴포넌트
    UPROPERTY()
        UAudioComponent* BGMComponent;

    // 현재 게임의 플레이어 컨트롤러 인스턴스
    UPROPERTY()
        APlayerController* PlayerController;

    // 게임의 인스턴스. 여기서는 UMyGameInstance 타입으로 사용
    UPROPERTY()
        UMyGameInstance* GameInstance;

};
