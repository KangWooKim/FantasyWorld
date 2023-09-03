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


// ANormalLevelHUD Ŭ������ �������Դϴ�. Ư���� �ʱ�ȭ�� �����ϴ�.
ANormalLevelHUD::ANormalLevelHUD()
{
}

// BeginPlay �޼����Դϴ�. ������ ���۵Ǹ� ȣ��˴ϴ�.
void ANormalLevelHUD::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay�� ȣ���մϴ�.
	Super::BeginPlay();

	// ���� ���带 �����ɴϴ�.
	World = GetWorld();

	// �÷��̾� ��Ʈ�ѷ��� �������ų� �̹� �����Ǿ� �ִٸ� �״�� �Ӵϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// ���� �ν��Ͻ��� �������ų� �̹� �����Ǿ� �ִٸ� �״�� �Ӵϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// MissionPreviewOverlay ������ �����ϰ� ȭ�鿡 �߰��մϴ�.
	if (World && PlayerController && MissionPreviewOverlayClass) {
		MissionPreviewOverlay = CreateWidget<UMissionPreviewOverlay>(PlayerController, MissionPreviewOverlayClass);
		MissionPreviewOverlay->AddToViewport();
		MissionPreviewOverlay->SetVisibility(ESlateVisibility::Visible);
	}

	// 5�� �Ŀ� MissionPreviewOverlay�� �����ϴ� Ÿ�̸Ӹ� �����մϴ�.
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ANormalLevelHUD::RemoveMissionPreviewOverlay, 5.f, false);

	// �ٸ� Overlay���� �����ϰ� �����մϴ�.
	CreateCombatOverlay();
	ShowCombatOverlay();
	CreateSettingsOverlay();
	HideSettingsOverlay();
	CreateMissionClearOverlay();
	HideMissionClearOverlay();
	CreateDieMenuOverlay();
	HideDieMenuOverlay();
}

// MissionPreviewOverlay�� �����ϴ� �޼����Դϴ�.
void ANormalLevelHUD::RemoveMissionPreviewOverlay()
{
	if (MissionPreviewOverlay) {
		MissionPreviewOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// CombatOverlay�� �����ϴ� �޼����Դϴ�.
void ANormalLevelHUD::CreateCombatOverlay()
{
	// �÷��̾� ��Ʈ�ѷ��� �������ų� �̹� �����Ǿ� �ִٸ� �״�� �Ӵϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// CombatOverlay ������ �����ϰ� ȭ�鿡 �߰��մϴ�.
	if (World && PlayerController && CombatOverlayClass) {
		CombatOverlay = CreateWidget<UCombatOverlay>(PlayerController, CombatOverlayClass);
		CombatOverlay->AddToViewport();
	}
}


// CombatOverlay�� ȭ�鿡 ���̰� �ϴ� �޼����Դϴ�.
void ANormalLevelHUD::ShowCombatOverlay()
{
	// CombatOverlay�� nullptr�� �ƴ϶�� ���̰� �����մϴ�.
	if (CombatOverlay) {
		CombatOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

// CombatOverlay�� ȭ�鿡�� ����� �޼����Դϴ�.
void ANormalLevelHUD::HideCombatOverlay()
{
	// CombatOverlay�� nullptr�� �ƴ϶�� ����� �����մϴ�.
	if (CombatOverlay) {
		CombatOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// MissionClearOverlay�� �����ϴ� �޼����Դϴ�.
void ANormalLevelHUD::CreateMissionClearOverlay()
{
	// �÷��̾� ��Ʈ�ѷ��� Ȯ���ϰų� �����մϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// MissionClearOverlay�� �����ϰ� ȭ�鿡 �߰��մϴ�.
	if (World && PlayerController && MissionClearOverlayClass) {
		MissionClearOverlay = CreateWidget<UMissionClearOverlay>(PlayerController, MissionClearOverlayClass);
		MissionClearOverlay->AddToViewport();
	}
}

// MissionClearOverlay�� ���̰� �ϰ� �ؽ�Ʈ ����� ������Ʈ�ϴ� �޼����Դϴ�.
void ANormalLevelHUD::ShowMissionClearOverlay(int32 SlayedEnemy)
{
	// MissionClearOverlay�� nullptr�� �ƴ϶�� ���̰� �����ϰ� �ؽ�Ʈ ����� ������Ʈ�մϴ�.
	if (MissionClearOverlay) {
		MissionClearOverlay->SetVisibility(ESlateVisibility::Visible);
		MissionClearOverlay->UpdateTextBlock(SlayedEnemy);
	}
}

// MissionClearOverlay�� ȭ�鿡�� ����� �޼����Դϴ�.
void ANormalLevelHUD::HideMissionClearOverlay()
{
	// MissionClearOverlay�� nullptr�� �ƴ϶�� ����� �����մϴ�.
	if (MissionClearOverlay) {
		MissionClearOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// DieMenuOverlay�� �����ϴ� �޼����Դϴ�.
void ANormalLevelHUD::CreateDieMenuOverlay()
{
	// �÷��̾� ��Ʈ�ѷ��� Ȯ���ϰų� �����մϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;

	// DieMenuOverlay�� �����ϰ� ȭ�鿡 �߰��մϴ�.
	if (World && PlayerController && DieMenuOverlayClass) {
		DieMenuOverlay = CreateWidget<UDieMenuOverlay>(PlayerController, DieMenuOverlayClass);
		DieMenuOverlay->AddToViewport();
	}
}


// ShowDieMenuOverlay �޼���: ���� �� ��Ÿ���� �޴�(��������)�� ȭ�鿡 �����ݴϴ�.
// SlayedEnemy: �÷��̾ �ױ� ���� ���� ���� ��
// LeftEnemy: �÷��̾ ���� �� ���� ���� ��
void ANormalLevelHUD::ShowDieMenuOverlay(int32 SlayedEnemy, int32 LeftEnemy)
{
	// DieMenuOverlay�� nullptr�� �ƴ϶��
	if (DieMenuOverlay) {
		// ���������� ���ü��� 'Visible'�� �����Ͽ� ȭ�鿡 ���̰� ��
		DieMenuOverlay->SetVisibility(ESlateVisibility::Visible);
		// �ױ� ���� ���� ���� ���� ���� ���� ������Ʈ ��
		DieMenuOverlay->UpdateTextBlock(SlayedEnemy, LeftEnemy);
	}
}

// HideDieMenuOverlay �޼���: ���� �� ��Ÿ���� �޴�(��������)�� ȭ�鿡�� ����ϴ�.
void ANormalLevelHUD::HideDieMenuOverlay()
{
	// DieMenuOverlay�� nullptr�� �ƴ϶��
	if (DieMenuOverlay) {
		// ���������� ���ü��� 'Hidden'���� �����Ͽ� ȭ�鿡�� ����
		DieMenuOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// CreateSettingsOverlay �޼���: ���� �޴�(��������)�� �����մϴ�.
void ANormalLevelHUD::CreateSettingsOverlay()
{
	// PlayerController�� nullptr�̸�, ù ��° �÷��̾� ��Ʈ�ѷ��� ������
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(World->GetFirstPlayerController()) : PlayerController;
	// World, PlayerController, �׸��� SettingsOverlayClass�� ��� nullptr�� �ƴ� ���
	if (World && PlayerController && SettingsOverlayClass) {
		// ���� �������̸� �����ϰ� ����Ʈ�� �߰���
		SettingsOverlay = CreateWidget<USettingsOverlay>(PlayerController, SettingsOverlayClass);
		SettingsOverlay->AddToViewport();
	}
}

// ShowSettingsOverlay �޼���: ���� �޴�(��������)�� ȭ�鿡 �����ݴϴ�.
void ANormalLevelHUD::ShowSettingsOverlay()
{
	// GameInstance�� nullptr�̸�, ���� ���� �ν��Ͻ��� ������
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	// SettingsOverlay�� nullptr�� �ƴ϶��
	if (SettingsOverlay) {
		// ���������� ���ü��� 'Visible'�� ����
		SettingsOverlay->SetVisibility(ESlateVisibility::Visible);
		// GameInstance�� nullptr�� �ƴ϶��
		if (GameInstance) {
			// �� �����̴��� ���� �ν��Ͻ����� ������ ������ ����
			SettingsOverlay->BGMVolume->SetValue(GameInstance->GetBGMVolume());
			SettingsOverlay->EffectSoundVolume->SetValue(GameInstance->GetEffectVolume());
			SettingsOverlay->MouseSensitivitySlider->SetValue(GameInstance->GetMouseSensitivity() / 5);
		}
	}
}

// HideSettingsOverlay �޼���: ���� �޴�(��������)�� ȭ�鿡�� ����ϴ�.
void ANormalLevelHUD::HideSettingsOverlay()
{
	// SettingsOverlay�� nullptr�� �ƴ϶��
	if (SettingsOverlay) {
		// ���������� ���ü��� 'Hidden'���� �����Ͽ� ȭ�鿡�� ����
		SettingsOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
