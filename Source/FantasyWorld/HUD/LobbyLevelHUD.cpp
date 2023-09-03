// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelHUD.h"
#include "LobbyLevelMenuOverlay.h"
#include "SelectMapOverlay.h"
#include "SelectCharacterOverlay.h"
#include "SettingsOverlay.h"

// BeginPlay �޼���� ������ �÷��̰� ���۵� �� ȣ��˴ϴ�.
void ALobbyLevelHUD::BeginPlay()
{
	Super::BeginPlay();  // �θ� Ŭ������ BeginPlay�� ȣ���Ͽ� �⺻ ������ �����մϴ�.

	// ���� ��ü�� ����ϴ�. ���� ��ü�� null�̸� GetWorld�� ȣ���Ͽ� ����ϴ�.
	World = World == nullptr ? GetWorld() : World;

	// ���� ��ü�� ��ȿ�ϸ� �����մϴ�.
	if (World) {
		// �÷��̾� ��Ʈ�ѷ��� ����ϴ�. �÷��̾� ��Ʈ�ѷ��� null�̸� ù ��° �÷��̾� ��Ʈ�ѷ��� ����ϴ�.
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;

		// �÷��̾� ��Ʈ�ѷ��� LobbyLevelMenuOverlayClass�� ��ȿ�ϸ� ������ �����ϰ� ����Ʈ�� �߰��մϴ�.
		if (PlayerController && LobbyLevelMenuOverlayClass) {
			LobbyLevelMenuOverlay = CreateWidget<ULobbyLevelMenuOverlay>(PlayerController, LobbyLevelMenuOverlayClass);
			LobbyLevelMenuOverlay->AddToViewport();
		}
	}

	// ���� �޼������ ������ �������̸� �����ϰ� ����ϴ�.
	CreateSelectMapOverlay();
	HideSelectMapOverlay();

	CreateSelectCharacterOverlay();
	HideSelectCharacterOverlay();

	CreateSettingsOverlay();
	HideSettingsOverlay();
}

// SelectMapOverlay ������ �����մϴ�.
void ALobbyLevelHUD::CreateSelectMapOverlay()
{
	// ����, �÷��̾� ��Ʈ�ѷ�, SelectMapOverlayClass�� ��� ��ȿ�ϸ� ������ �����ϰ� ����Ʈ�� �߰��մϴ�.
	if (World && PlayerController && SelectMapOverlayClass) {
		SelectMapOverlay = CreateWidget<USelectMapOverlay>(PlayerController, SelectMapOverlayClass);
		SelectMapOverlay->AddToViewport();
	}
}

// SelectMapOverlay�� ���̰� �մϴ�.
void ALobbyLevelHUD::ShowSelectMapOverlay()
{
	// SelectMapOverlay�� ��ȿ�ϸ� ���ü��� �����մϴ�.
	if (SelectMapOverlay) {
		SelectMapOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

// SelectMapOverlay�� ����ϴ�.
void ALobbyLevelHUD::HideSelectMapOverlay()
{
	// SelectMapOverlay�� ��ȿ�ϸ� ���ü��� �����մϴ�.
	if (SelectMapOverlay) {
		SelectMapOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// SelectCharacterOverlay ������ �����մϴ�.
void ALobbyLevelHUD::CreateSelectCharacterOverlay()
{
	// ����, �÷��̾� ��Ʈ�ѷ�, SelectCharacterOverlayClass�� ��� ��ȿ�ϸ� ������ �����ϰ� ����Ʈ�� �߰��մϴ�.
	if (World && PlayerController && SelectCharacterOverlayClass) {
		SelectCharacterOverlay = CreateWidget<USelectCharacterOverlay>(PlayerController, SelectCharacterOverlayClass);
		SelectCharacterOverlay->AddToViewport();
	}
}


// SelectCharacterOverlay�� ���̰� �ϴ� �Լ��Դϴ�.
void ALobbyLevelHUD::ShowSelectCharacterOverlay()
{
	// SelectCharacterOverlay ��ü�� ��ȿ�� ���
	if (SelectCharacterOverlay) {
		// SelectCharacterOverlay�� ���ü��� 'Visible'�� �����մϴ�.
		SelectCharacterOverlay->SetVisibility(ESlateVisibility::Visible);
	}
}

// SelectCharacterOverlay�� ����� �Լ��Դϴ�.
void ALobbyLevelHUD::HideSelectCharacterOverlay()
{
	// SelectCharacterOverlay ��ü�� ��ȿ�� ���
	if (SelectCharacterOverlay) {
		// SelectCharacterOverlay�� ���ü��� 'Hidden'���� �����մϴ�.
		SelectCharacterOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

// SettingsOverlay ������ �����ϴ� �Լ��Դϴ�.
void ALobbyLevelHUD::CreateSettingsOverlay()
{
	// World, PlayerController, �׸��� SettingsOverlayClass�� ��� ��ȿ�� ���
	if (World && PlayerController && SettingsOverlayClass) {
		// SettingsOverlay ������ �����ϰ� ����Ʈ�� �߰��մϴ�.
		SettingsOverlay = CreateWidget<USettingsOverlay>(PlayerController, SettingsOverlayClass);
		SettingsOverlay->AddToViewport();
	}
}

// SettingsOverlay�� ���̰� �ϴ� �Լ��Դϴ�.
void ALobbyLevelHUD::ShowSettingsOverlay()
{
	// SettingsOverlay ��ü�� ��ȿ�� ���
	if (SettingsOverlay) {
		// SettingsOverlay�� ���ü��� 'Visible'�� �����մϴ�.
		SettingsOverlay->SetVisibility(ESlateVisibility::Visible);

		// 'Continue' ��ư�� ����ϴ�.
		SettingsOverlay->HideContinueButton();
	}
}

// SettingsOverlay�� ����� �Լ��Դϴ�.
void ALobbyLevelHUD::HideSettingsOverlay()
{
	// SettingsOverlay ��ü�� ��ȿ�� ���
	if (SettingsOverlay) {
		// SettingsOverlay�� ���ü��� 'Hidden'���� �����մϴ�.
		SettingsOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}
