// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// ĳ������ ������ �����ϱ� ���� ������(enum class). Blueprint������ ��밡���ϵ��� ����
UENUM(BlueprintType)
enum class ECharacterTypes : uint8 {
    Countess UMETA(DisplayName = "Countess"),  // Countess ĳ���� Ÿ��
    Phase UMETA(DisplayName = "Phase"),        // Phase ĳ���� Ÿ��
    Character3 UMETA(DisplayName = "Character3") // Character3 ĳ���� Ÿ��
};

// UMyGameInstance Ŭ����. UGameInstance�� ��� ����.
UCLASS()
class FANTASYWORLD_API UMyGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:

    // ����ڰ� ������ ĳ���͸� �����ϴ� �Լ�
    // CharacterType: �����ϰ��� �ϴ� ĳ������ Ÿ��
    UFUNCTION()
        void SetSelectedCharacter(ECharacterTypes CharacterType);

    // ���� �� �̸��� �����ϴ� �Լ�
    // name: �����ϰ��� �ϴ� ���� �̸�
    UFUNCTION()
        void SetMapName(FString name);

    // ����ڰ� � ĳ���͸� �����ߴ��� ��ȯ�ϴ� �Լ�
    UFUNCTION()
        ECharacterTypes GetSelectedCharacter();

    // ���� ���õ� �� �̸��� ��ȯ�ϴ� �Լ�
    UFUNCTION()
        FString GetSelectedMap();

    // ������ �����ϴ� �Լ�
    UFUNCTION()
        void StartLevel();

    // ���� óġ�� ���� ��ȯ�ϴ� �Լ�
    FORCEINLINE int32 GetScore() { return Enemy; }

    // ��� ���� ������ ��ȯ�ϴ� �Լ�
    FORCEINLINE float GetBGMVolume() { return BGMVolume; }

    // ȿ���� ������ ��ȯ�ϴ� �Լ�
    FORCEINLINE float GetEffectVolume() { return EffectVolume; }

    // ���콺 �ΰ����� ��ȯ�ϴ� �Լ�
    FORCEINLINE float GetMouseSensitivity() const { return MouseSensitivity; }

    // ����� ������ �����ϴ� �Լ�
    // v: �����ϰ��� �ϴ� ���� ��
    UFUNCTION()
        void SetBGMVolume(float v);

    // ȿ���� ������ �����ϴ� �Լ�
    // v: �����ϰ��� �ϴ� ���� ��
    UFUNCTION()
        void SetEffectVolume(float v);

    // ���콺 �ΰ����� �����ϴ� �Լ�
    // value: �����ϰ��� �ϴ� �ΰ��� ��
    UFUNCTION()
        void SetMouseSensitivity(float value);

private:

    // ����ڰ� ������ ĳ���� ������ �����ϴ� ����
    UPROPERTY()
        ECharacterTypes SelectedCharacter;

    // ���� ���õ� ���� �� �̸��� �����ϴ� ����
    UPROPERTY()
        FString MapName;

    // ���� óġ�� ���� �����ϴ� ����
    UPROPERTY()
        int32 Enemy = 0;

    // ����� ������ �����ϴ� ����
    UPROPERTY()
        float BGMVolume = 1.f;

    // ȿ���� ������ �����ϴ� ����
    UPROPERTY()
        float EffectVolume = 1.f;

    // ���콺 �ΰ����� �����ϴ� ����
    UPROPERTY(EditAnywhere)
        float MouseSensitivity = 1.f;
};
