// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

// 플레이 하고자 하는 캐릭터를 저장하는 함수
void UMyGameInstance::SetSelectedCharacter(ECharacterTypes CharacterType)
{
	SelectedCharacter = CharacterType;
}

// 플레이 하고자 하는 캐릭터를 저장하는 변수
ECharacterTypes UMyGameInstance::GetSelectedCharacter()
{
	return SelectedCharacter;
}
