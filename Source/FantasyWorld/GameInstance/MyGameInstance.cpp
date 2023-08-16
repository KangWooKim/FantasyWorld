// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

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

FString UMyGameInstance::GetSelectedMap()
{
	return MapName;
}

void UMyGameInstance::StartLevel()
{
	if (GetWorld()) {
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));
	}
}

void UMyGameInstance::SetBGMVolume(float v)
{
	BGMVolume = v;
}

void UMyGameInstance::SetEffectVolume(float v)
{
	EffectVolume = v;
}

void UMyGameInstance::SetMouseSensitivity(float value)
{
	MouseSensitivity = value;
}

void UMyGameInstance::SetMapName(FString name) {
	MapName = name;
}