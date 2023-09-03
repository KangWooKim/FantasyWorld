// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelHUD.h"
#include "LobbyLevelMenuOverlay.h"
#include "SelectMapOverlay.h"
#include "SelectCharacterOverlay.h"
#include "SettingsOverlay.h"

// BeginPlay 메서드는 게임의 플레이가 시작될 때 호출됩니다.
void ALobbyLevelHUD::BeginPlay()
{
	Super::BeginPlay();  // 부모 클래스의 BeginPlay를 호출하여 기본 로직을 실행합니다.

	// 월드 객체를 얻습니다. 월드 객체가 null이면 GetWorld를 호출하여 얻습니다.
	World = World == nullptr ? GetWorld() : World;

	// 월드 객체가 유효하면 실행합니다.
	if (World) {
		// 플레이어 컨트롤러를 얻습니다. 플레이어 컨트롤러가 null이면 첫 번째 플레이어 컨트롤러를 얻습니다.
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;

		// 플레이어 컨트롤러와 LobbyLevelMenuOverlayClass가 유효하면 위젯을 생성하고 뷰포트에 추가합니다.
		if (PlayerController && LobbyLevelMenuOverlayClass) {
			LobbyLevelMenuOverlay = CreateWidget<ULobbyLevelMenuOverlay>(PlayerController, LobbyLevelMenuOverlayClass);
			LobbyLevelMenuOverlay->AddToViewport();
		}
	}

	// 다음 메서드들은 각각의 오버레이를 생성하고 숨깁니다.
	CreateSelectMapOverlay();
	HideSelectMapOverlay();

	CreateSelectCharacterOverlay();
	HideSelectCharacterOverlay();

	CreateSettingsOverlay();
	HideSettingsOverlay();
}

// SelectMapOverlay 위젯을 생성합니다.
void ALobbyLevelHUD::CreateSelectMapOverlay()
{
	// 월드, 플레이어 컨트롤러, SelectMapOverlayClass가 모두 유효하면 위젯을 생성하고 뷰포트에 추가합니다.
	if (World && PlayerController && SelectMapOverlayClass) {
		SelectMapOverlay = CreateWidget<USelectMapOverlay>(PlayerController, SelectMapOverlayClass);
		SelectMapOverlay->AddToViewport();
	}
}

// SelectMapOverlay를 보이게 합니다.
void ALobbyLevelHUD::ShowSelectMapOverlay()
{
	// SelectMapOverlay가 유효하면 가시성을 설정합니다.
	if (SelectMapOverlay) {
		SelectMapOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

// SelectMapOverlay를 숨깁니다.
void ALobbyLevelHUD::HideSelectMapOverlay()
{
	// SelectMapOverlay가 유효하면 가시성을 설정합니다.
	if (SelectMapOverlay) {
		SelectMapOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// SelectCharacterOverlay 위젯을 생성합니다.
void ALobbyLevelHUD::CreateSelectCharacterOverlay()
{
	// 월드, 플레이어 컨트롤러, SelectCharacterOverlayClass가 모두 유효하면 위젯을 생성하고 뷰포트에 추가합니다.
	if (World && PlayerController && SelectCharacterOverlayClass) {
		SelectCharacterOverlay = CreateWidget<USelectCharacterOverlay>(PlayerController, SelectCharacterOverlayClass);
		SelectCharacterOverlay->AddToViewport();
	}
}


// SelectCharacterOverlay를 보이게 하는 함수입니다.
void ALobbyLevelHUD::ShowSelectCharacterOverlay()
{
	// SelectCharacterOverlay 객체가 유효한 경우
	if (SelectCharacterOverlay) {
		// SelectCharacterOverlay의 가시성을 'Visible'로 설정합니다.
		SelectCharacterOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

// SelectCharacterOverlay를 숨기는 함수입니다.
void ALobbyLevelHUD::HideSelectCharacterOverlay()
{
	// SelectCharacterOverlay 객체가 유효한 경우
	if (SelectCharacterOverlay) {
		// SelectCharacterOverlay의 가시성을 'Hidden'으로 설정합니다.
		SelectCharacterOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// SettingsOverlay 위젯을 생성하는 함수입니다.
void ALobbyLevelHUD::CreateSettingsOverlay()
{
	// World, PlayerController, 그리고 SettingsOverlayClass가 모두 유효한 경우
	if (World && PlayerController && SettingsOverlayClass) {
		// SettingsOverlay 위젯을 생성하고 뷰포트에 추가합니다.
		SettingsOverlay = CreateWidget<USettingsOverlay>(PlayerController, SettingsOverlayClass);
		SettingsOverlay->AddToViewport();
	}
}

// SettingsOverlay를 보이게 하는 함수입니다.
void ALobbyLevelHUD::ShowSettingsOverlay()
{
	// SettingsOverlay 객체가 유효한 경우
	if (SettingsOverlay) {
		// SettingsOverlay의 가시성을 'Visible'로 설정합니다.
		SettingsOverlay->SetVisibility(ESlateVisibility::Visible);

		// 'Continue' 버튼을 숨깁니다.
		SettingsOverlay->HideContinueButton();
	}
}

// SettingsOverlay를 숨기는 함수입니다.
void ALobbyLevelHUD::HideSettingsOverlay()
{
	// SettingsOverlay 객체가 유효한 경우
	if (SettingsOverlay) {
		// SettingsOverlay의 가시성을 'Hidden'으로 설정합니다.
		SettingsOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
