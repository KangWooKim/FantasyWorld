// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// �÷��� �ϰ��� �ϴ� ĳ���͸� �����ϴ� �Լ�
// CharacterType: �����Ϸ��� ĳ���� Ÿ��
void UMyGameInstance::SetSelectedCharacter(ECharacterTypes CharacterType)
{
	SelectedCharacter = CharacterType;  // ������ ĳ���� Ÿ���� ��� ������ ����
}

// �÷��� �ϰ��� �ϴ� ĳ���͸� ��ȯ�ϴ� �Լ�
ECharacterTypes UMyGameInstance::GetSelectedCharacter()
{
	return SelectedCharacter;  // ������ ĳ���� Ÿ���� ��ȯ
}

// ���� ���õ� �� �̸��� ��ȯ�ϴ� �Լ�
FString UMyGameInstance::GetSelectedMap()
{
	return MapName;  // ���� ������ �� �̸��� ��ȯ
}

// ���� ������ ���� �����ϴ� �Լ�
void UMyGameInstance::StartLevel()
{
	if (GetWorld()) {  // ���� ��ü�� ������ ���
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));  // ������ ���� ����
	}
}

// ����� ������ �����ϴ� �Լ�
// v: �����Ϸ��� ���� ��
void UMyGameInstance::SetBGMVolume(float v)
{
	BGMVolume = v;  // ����� ������ ��� ������ ����
}

// ȿ���� ������ �����ϴ� �Լ�
// v: �����Ϸ��� ���� ��
void UMyGameInstance::SetEffectVolume(float v)
{
	EffectVolume = v;  // ȿ���� ������ ��� ������ ����
}

// ���콺 �ΰ����� �����ϴ� �Լ�
// value: �����Ϸ��� ���콺 �ΰ��� ��
void UMyGameInstance::SetMouseSensitivity(float value)
{
	MouseSensitivity = value;  // ���콺 �ΰ����� ��� ������ ����
}

// �� �̸��� �����ϴ� �Լ�
// name: �����Ϸ��� �� �̸�
void UMyGameInstance::SetMapName(FString name)
{
	MapName = name;  // �� �̸��� ��� ������ ����
}
