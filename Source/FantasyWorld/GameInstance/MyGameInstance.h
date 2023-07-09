// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

// ������ ĳ���͸� �����ϱ� ���� enum class
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

	// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� �Լ�
	// @parameter
	// ECharacterTypes CharacterType : ������ ĳ����
	void SetSelectedCharacter(ECharacterTypes CharacterType);

	// ��� ĳ���͸� ������� �ľ��ϱ� ���� �Լ�
	ECharacterTypes GetSelectedCharacter();


private:

	// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� ����
	ECharacterTypes SelectedCharacter;
};
