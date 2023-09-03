// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalLevelHUD.h"
#include "MissionClearOverlay.h"
#include "DieMenuOverlay.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/HUD/MissionPreviewOverlay.h"
#include "CombatOverlay.h"
#include "SettingsOverlay.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Slider.h"


// ANormalLevelHUD 클래스의 생성자입니다. 특별한 초기화가 없습니다.
ANormalLevelHUD::ANormalLevelHUD()
{
}

// BeginPlay 메서드입니다. 게임이 시작되면 호출됩니다.
void ANormalLevelHUD::BeginPlay()
{
	// 부모 클래스의 BeginPlay를 호출합니다.
	Super::BeginPlay();

	// 현재 월드를 가져옵니다.
	World = GetWorld();

	// 플레이어 컨트롤러를 가져오거나 이미 설정되어 있다면 그대로 둡니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// 게임 인스턴스를 가져오거나 이미 설정되어 있다면 그대로 둡니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// MissionPreviewOverlay 위젯을 생성하고 화면에 추가합니다.
	if (World && PlayerController && MissionPreviewOverlayClass) {
		MissionPreviewOverlay = CreateWidget<UMissionPreviewOverlay>(PlayerController, MissionPreviewOverlayClass);
		MissionPreviewOverlay->AddToViewport();
		MissionPreviewOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	// 5초 후에 MissionPreviewOverlay를 제거하는 타이머를 설정합니다.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANormalLevelHUD::RemoveMissionPreviewOverlay, 5.f, false);

	// 다른 Overlay들을 생성하고 설정합니다.
	CreateCombatOverlay();
	ShowCombatOverlay();
	CreateSettingsOverlay();
	HideSettingsOverlay();
	CreateMissionClearOverlay();
	HideMissionClearOverlay();
	CreateDieMenuOverlay();
	HideDieMenuOverlay();
}

// MissionPreviewOverlay를 제거하는 메서드입니다.
void ANormalLevelHUD::RemoveMissionPreviewOverlay()
{
	if (MissionPreviewOverlay) {
		MissionPreviewOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// CombatOverlay를 생성하는 메서드입니다.
void ANormalLevelHUD::CreateCombatOverlay()
{
	// 플레이어 컨트롤러를 가져오거나 이미 설정되어 있다면 그대로 둡니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// CombatOverlay 위젯을 생성하고 화면에 추가합니다.
	if (World && PlayerController && CombatOverlayClass) {
		CombatOverlay = CreateWidget<UCombatOverlay>(PlayerController, CombatOverlayClass);
		CombatOverlay->AddToViewport();
	}
}


// CombatOverlay를 화면에 보이게 하는 메서드입니다.
void ANormalLevelHUD::ShowCombatOverlay()
{
	// CombatOverlay가 nullptr이 아니라면 보이게 설정합니다.
	if (CombatOverlay) {
		CombatOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

// CombatOverlay를 화면에서 숨기는 메서드입니다.
void ANormalLevelHUD::HideCombatOverlay()
{
	// CombatOverlay가 nullptr이 아니라면 숨기게 설정합니다.
	if (CombatOverlay) {
		CombatOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// MissionClearOverlay를 생성하는 메서드입니다.
void ANormalLevelHUD::CreateMissionClearOverlay()
{
	// 플레이어 컨트롤러를 확인하거나 설정합니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// MissionClearOverlay를 생성하고 화면에 추가합니다.
	if (World && PlayerController && MissionClearOverlayClass) {
		MissionClearOverlay = CreateWidget<UMissionClearOverlay>(PlayerController, MissionClearOverlayClass);
		MissionClearOverlay->AddToViewport();
	}
}

// MissionClearOverlay를 보이게 하고 텍스트 블록을 업데이트하는 메서드입니다.
void ANormalLevelHUD::ShowMissionClearOverlay(int32 SlayedEnemy)
{
	// MissionClearOverlay가 nullptr이 아니라면 보이게 설정하고 텍스트 블록을 업데이트합니다.
	if (MissionClearOverlay) {
		MissionClearOverlay->SetVisibility(ESlateVisibility::Visible);
		MissionClearOverlay->UpdateTextBlock(SlayedEnemy);
	}
}

// MissionClearOverlay를 화면에서 숨기는 메서드입니다.
void ANormalLevelHUD::HideMissionClearOverlay()
{
	// MissionClearOverlay가 nullptr이 아니라면 숨기게 설정합니다.
	if (MissionClearOverlay) {
		MissionClearOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// DieMenuOverlay를 생성하는 메서드입니다.
void ANormalLevelHUD::CreateDieMenuOverlay()
{
	// 플레이어 컨트롤러를 확인하거나 설정합니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// DieMenuOverlay를 생성하고 화면에 추가합니다.
	if (World && PlayerController && DieMenuOverlayClass) {
		DieMenuOverlay = CreateWidget<UDieMenuOverlay>(PlayerController, DieMenuOverlayClass);
		DieMenuOverlay->AddToViewport();
	}
}


// ShowDieMenuOverlay 메서드: 죽은 후 나타나는 메뉴(오버레이)를 화면에 보여줍니다.
// SlayedEnemy: 플레이어가 죽기 전에 죽인 적의 수
// LeftEnemy: 플레이어가 죽은 후 남은 적의 수
void ANormalLevelHUD::ShowDieMenuOverlay(int32 SlayedEnemy, int32 LeftEnemy)
{
	// DieMenuOverlay가 nullptr이 아니라면
	if (DieMenuOverlay) {
		// 오버레이의 가시성을 'Visible'로 설정하여 화면에 보이게 함
		DieMenuOverlay->SetVisibility(ESlateVisibility::Visible);
		// 죽기 전에 죽인 적과 남은 적의 수를 업데이트 함
		DieMenuOverlay->UpdateTextBlock(SlayedEnemy, LeftEnemy);
	}
}

// HideDieMenuOverlay 메서드: 죽은 후 나타나는 메뉴(오버레이)를 화면에서 숨깁니다.
void ANormalLevelHUD::HideDieMenuOverlay()
{
	// DieMenuOverlay가 nullptr이 아니라면
	if (DieMenuOverlay) {
		// 오버레이의 가시성을 'Hidden'으로 설정하여 화면에서 숨김
		DieMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// CreateSettingsOverlay 메서드: 설정 메뉴(오버레이)를 생성합니다.
void ANormalLevelHUD::CreateSettingsOverlay()
{
	// PlayerController가 nullptr이면, 첫 번째 플레이어 컨트롤러를 가져옴
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	// World, PlayerController, 그리고 SettingsOverlayClass가 모두 nullptr이 아닐 경우
	if (World && PlayerController && SettingsOverlayClass) {
		// 설정 오버레이를 생성하고 뷰포트에 추가함
		SettingsOverlay = CreateWidget<USettingsOverlay>(PlayerController, SettingsOverlayClass);
		SettingsOverlay->AddToViewport();
	}
}

// ShowSettingsOverlay 메서드: 설정 메뉴(오버레이)를 화면에 보여줍니다.
void ANormalLevelHUD::ShowSettingsOverlay()
{
	// GameInstance가 nullptr이면, 현재 게임 인스턴스를 가져옴
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	// SettingsOverlay가 nullptr이 아니라면
	if (SettingsOverlay) {
		// 오버레이의 가시성을 'Visible'로 설정
		SettingsOverlay->SetVisibility(ESlateVisibility::Visible);
		// GameInstance가 nullptr이 아니라면
		if (GameInstance) {
			// 각 슬라이더에 게임 인스턴스에서 설정한 값들을 적용
			SettingsOverlay->BGMVolume->SetValue(GameInstance->GetBGMVolume());
			SettingsOverlay->EffectSoundVolume->SetValue(GameInstance->GetEffectVolume());
			SettingsOverlay->MouseSensitivitySlider->SetValue(GameInstance->GetMouseSensitivity() / 5);
		}
	}
}

// HideSettingsOverlay 메서드: 설정 메뉴(오버레이)를 화면에서 숨깁니다.
void ANormalLevelHUD::HideSettingsOverlay()
{
	// SettingsOverlay가 nullptr이 아니라면
	if (SettingsOverlay) {
		// 오버레이의 가시성을 'Hidden'으로 설정하여 화면에서 숨김
		SettingsOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
