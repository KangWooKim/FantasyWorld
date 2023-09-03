// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

// 플레이 하고자 하는 캐릭터를 저장하는 함수
// CharacterType: 설정하려는 캐릭터 타입
void UMyGameInstance::SetSelectedCharacter(ECharacterTypes CharacterType)
{
	SelectedCharacter = CharacterType;  // 선택한 캐릭터 타입을 멤버 변수에 저장
}

// 플레이 하고자 하는 캐릭터를 반환하는 함수
ECharacterTypes UMyGameInstance::GetSelectedCharacter()
{
	return SelectedCharacter;  // 선택한 캐릭터 타입을 반환
}

// 현재 선택된 맵 이름을 반환하는 함수
FString UMyGameInstance::GetSelectedMap()
{
	return MapName;  // 현재 설정된 맵 이름을 반환
}

// 현재 설정된 맵을 시작하는 함수
void UMyGameInstance::StartLevel()
{
	if (GetWorld()) {  // 월드 객체가 존재할 경우
		UGameplayStatics::OpenLevel(GetWorld(), FName(*MapName));  // 설정된 맵을 열기
	}
}

// 배경음 볼륨을 설정하는 함수
// v: 설정하려는 볼륨 값
void UMyGameInstance::SetBGMVolume(float v)
{
	BGMVolume = v;  // 배경음 볼륨을 멤버 변수에 저장
}

// 효과음 볼륨을 설정하는 함수
// v: 설정하려는 볼륨 값
void UMyGameInstance::SetEffectVolume(float v)
{
	EffectVolume = v;  // 효과음 볼륨을 멤버 변수에 저장
}

// 마우스 민감도를 설정하는 함수
// value: 설정하려는 마우스 민감도 값
void UMyGameInstance::SetMouseSensitivity(float value)
{
	MouseSensitivity = value;  // 마우스 민감도를 멤버 변수에 저장
}

// 맵 이름을 설정하는 함수
// name: 설정하려는 맵 이름
void UMyGameInstance::SetMapName(FString name)
{
	MapName = name;  // 맵 이름을 멤버 변수에 저장
}
