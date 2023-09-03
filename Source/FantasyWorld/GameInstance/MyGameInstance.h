// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// 캐릭터의 유형을 결정하기 위한 열거형(enum class). Blueprint에서도 사용가능하도록 설정
UENUM(BlueprintType)
enum class ECharacterTypes : uint8 {
    Countess UMETA(DisplayName = "Countess"),  // Countess 캐릭터 타입
    Phase UMETA(DisplayName = "Phase"),        // Phase 캐릭터 타입
    Character3 UMETA(DisplayName = "Character3") // Character3 캐릭터 타입
};

// UMyGameInstance 클래스. UGameInstance를 상속 받음.
UCLASS()
class FANTASYWORLD_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    // 사용자가 선택한 캐릭터를 설정하는 함수
    // CharacterType: 설정하고자 하는 캐릭터의 타입
    UFUNCTION()
        void SetSelectedCharacter(ECharacterTypes CharacterType);

    // 게임 맵 이름을 설정하는 함수
    // name: 설정하고자 하는 맵의 이름
    UFUNCTION()
        void SetMapName(FString name);

    // 사용자가 어떤 캐릭터를 선택했는지 반환하는 함수
    UFUNCTION()
        ECharacterTypes GetSelectedCharacter();

    // 현재 선택된 맵 이름을 반환하는 함수
    UFUNCTION()
        FString GetSelectedMap();

    // 레벨을 시작하는 함수
    UFUNCTION()
        void StartLevel();

    // 적을 처치한 수를 반환하는 함수
    FORCEINLINE int32 GetScore() { return Enemy; }

    // 배경 음악 볼륨을 반환하는 함수
    FORCEINLINE float GetBGMVolume() { return BGMVolume; }

    // 효과음 볼륨을 반환하는 함수
    FORCEINLINE float GetEffectVolume() { return EffectVolume; }

    // 마우스 민감도를 반환하는 함수
    FORCEINLINE float GetMouseSensitivity() const { return MouseSensitivity; }

    // 배경음 볼륨을 설정하는 함수
    // v: 설정하고자 하는 볼륨 값
    UFUNCTION()
        void SetBGMVolume(float v);

    // 효과음 볼륨을 설정하는 함수
    // v: 설정하고자 하는 볼륨 값
    UFUNCTION()
        void SetEffectVolume(float v);

    // 마우스 민감도를 설정하는 함수
    // value: 설정하고자 하는 민감도 값
    UFUNCTION()
        void SetMouseSensitivity(float value);

private:

    // 사용자가 선택한 캐릭터 정보를 저장하는 변수
    UPROPERTY()
        ECharacterTypes SelectedCharacter;

    // 현재 선택된 게임 맵 이름을 저장하는 변수
    UPROPERTY()
        FString MapName;

    // 적을 처치한 수를 저장하는 변수
    UPROPERTY()
        int32 Enemy = 0;

    // 배경음 볼륨을 저장하는 변수
    UPROPERTY()
        float BGMVolume = 1.f;

    // 효과음 볼륨을 저장하는 변수
    UPROPERTY()
        float EffectVolume = 1.f;

    // 마우스 민감도를 저장하는 변수
    UPROPERTY(EditAnywhere)
        float MouseSensitivity = 1.f;
};
