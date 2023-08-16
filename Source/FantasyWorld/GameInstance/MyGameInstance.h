// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// 선택한 캐릭터를 구분하기 위한 enum class
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

	// 플레이 하고자 하는 캐릭터를 저장하는 함수
	// @parameter
	// ECharacterTypes CharacterType : 설정할 캐릭터
	UFUNCTION()
	void SetSelectedCharacter(ECharacterTypes CharacterType);

	UFUNCTION()
	void SetMapName(FString name);

	// 어떠한 캐릭터를 골랐는지 파악하기 위한 함수
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

	// 플레이 하고자 하는 캐릭터를 저장하는 변수
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
