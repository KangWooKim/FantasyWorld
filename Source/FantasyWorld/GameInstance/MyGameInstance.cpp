// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� �Լ�
void UMyGameInstance::SetSelectedCharacter(ECharacterTypes CharacterType)
{
	SelectedCharacter = CharacterType;
}

// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� ����
ECharacterTypes UMyGameInstance::GetSelectedCharacter()
{
	return SelectedCharacter;
}
