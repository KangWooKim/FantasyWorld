// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// ������ ĳ���͸� �����ϱ� ���� enum class
UENUM(BlueprintType)
enum class ECharacterTypes : uint8 {
	Countess UMETA(DisplayName = "Countess"),
	Phase UMETA(DisplayName = "Phase"),
	Character3 UMETA(DisplayName = "Character3")
};

UCLASS()
class FANTASYWORLD_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� �Լ�
	// @parameter
	// ECharacterTypes CharacterType : ������ ĳ����
	UFUNCTION()
	void SetSelectedCharacter(ECharacterTypes CharacterType);

	UFUNCTION()
	void SetMapName(FString name);

	// ��� ĳ���͸� ������� �ľ��ϱ� ���� �Լ�
	UFUNCTION()
	ECharacterTypes GetSelectedCharacter();

	UFUNCTION()
	FString GetSelectedMap();

	UFUNCTION()
	void StartLevel();

	FORCEINLINE int32 GetScore() { return Enemy;  }

	FORCEINLINE float GetBGMVolume() { return BGMVolume; }

	FORCEINLINE float GetEffectVolume() { return EffectVolume; }

	FORCEINLINE float GetMouseSensitivity() const { return MouseSensitivity; }

	UFUNCTION()
	void SetBGMVolume(float v);

	UFUNCTION()
	void SetEffectVolume(float v);

	UFUNCTION()
	void SetMouseSensitivity(float value);

private:

	// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� ����
	UPROPERTY()
	ECharacterTypes SelectedCharacter;

	UPROPERTY()
	FString MapName;


	UPROPERTY()
	int32 Enemy = 0;

	UPROPERTY()
	float BGMVolume = 1.f;

	UPROPERTY()
	float EffectVolume = 1.f;

	UPROPERTY(EditAnywhere)
	float MouseSensitivity = 1.f;
};
