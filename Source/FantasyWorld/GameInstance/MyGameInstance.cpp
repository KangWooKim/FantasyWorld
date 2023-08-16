// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

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