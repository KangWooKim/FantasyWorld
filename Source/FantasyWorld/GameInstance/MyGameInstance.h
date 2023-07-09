// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// 선택한 캐릭터를 구분하기 위한 enum class
UENUM(BlueprintType)
enum class ECharacterTypes : uint8 {
	Character1 UMETA(DisplayName = "Character1"),
	Character2 UMETA(DisplayName = "Character2"),
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
	void SetSelectedCharacter(ECharacterTypes CharacterType);

	// 어떠한 캐릭터를 골랐는지 파악하기 위한 함수
	ECharacterTypes GetSelectedCharacter();


private:

	// 플레이 하고자 하는 캐릭터를 저장하는 변수
	ECharacterTypes SelectedCharacter;
};
