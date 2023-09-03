// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsOverlay.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/Slider.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/Button.h"
#include "LobbyLevelHUD.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "GameFramework/GameUserSettings.h"



// USettingsOverlay�� NativeConstruct �Լ�. �� �Լ��� Overlay�� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
void USettingsOverlay::NativeConstruct()
{
	// �θ� Ŭ������ NativeConstruct �Լ��� ȣ���մϴ�.
	Super::NativeConstruct();

	// GameInstance�� Controller�� null���� Ȯ���ϰ�, null�̸� ���� �Ҵ��մϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	Controller = Controller == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : Controller;

	// BGMVolume �����̴��� �����ϸ� ������ �ʱ�ȭ�ϰ� �̺�Ʈ�� ���ε��մϴ�.
	if (BGMVolume) {
		// �����̴��� ������ ������� �����մϴ�.
		BGMVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);

		// GameInstance�� �����ϸ� BGM ���� ���� �����ͼ� �����̴��� �����մϴ�.
		if (GameInstance) {
			BGMVolume->SetValue(GameInstance->GetBGMVolume());
		}
		else {
			// GameInstance�� ���ٸ� �⺻�� 1�� �����մϴ�.
			BGMVolume->SetValue(1.f);
		}

		// BGMVolume ���� ����� �� ȣ��� �Լ��� ���ε��մϴ�.
		BGMVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleBGMVolumeChanged);
	}

	// ���� �ڵ�� ���� BGMVolume�� ����� ���������� ����Ʈ ���� ������ ���� ���Դϴ�.
	if (EffectSoundVolume) {
		EffectSoundVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		if (GameInstance) {
			EffectSoundVolume->SetValue(GameInstance->GetEffectVolume());
		}
		else {
			EffectSoundVolume->SetValue(1.f);
		}

		EffectSoundVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleEffectSoundVolumeChanged);
	}

	// ���콺 �ΰ����� ���� �����̴� ����
	if (MouseSensitivitySlider) {
		MouseSensitivitySlider->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);
		if (GameInstance) {
			MouseSensitivitySlider->SetValue(GameInstance->GetMouseSensitivity() / 5);
		}
		else {
			MouseSensitivitySlider->SetValue(1.f);
		}
		MouseSensitivitySlider->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleMouseSensitivitySliderChanged);
	}

	// ���ػ� ��ư�� ���� �̺�Ʈ ���ε�
	if (ResolutionHigh) {
		ResolutionHigh->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionHighButtonClicked);
	}

	// �߰� �ػ� ��ư�� ���� �̺�Ʈ ���ε�
	if (ResolutionMiddle) {
		ResolutionMiddle->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionMiddleButtonClicked);
	}

	// ���ػ� ��ư�� ���� �̺�Ʈ ���ε�
	if (ResolutionLow) {
		ResolutionLow->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionLowButtonClicked);
	}

	// ��üȭ�� ��� ��ư�� ���� �̺�Ʈ ���ε�
	if (FullScreenMode) {
		FullScreenMode->OnClicked.AddDynamic(this, &USettingsOverlay::OnFullScreenModeButtonClicked);
	}

	// ������ ��� ��ư�� ���� �̺�Ʈ ���ε�
	if (WindowMode) {
		WindowMode->OnClicked.AddDynamic(this, &USettingsOverlay::OnWindowModeButtonClicked);
	}

	// ���� ��ư�� ���� �̺�Ʈ ���ε�
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnQuitButtonClicked);
	}

	// ��� ��ư�� ���� �̺�Ʈ ���ε�
	if (ContinueButton) {
		ContinueButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnContinueButtonClicked);
	}
}


// BGM ������ �����ϴ� �̺�Ʈ �ڵ鷯 �Լ��Դϴ�.
void USettingsOverlay::HandleBGMVolumeChanged(float value)
{
	// GameInstance�� null�̸� ���� �Ҵ��մϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance�� �����ϸ� BGM ������ �����մϴ�.
	if (GameInstance) {
		GameInstance->SetBGMVolume(value);
	}
}

// ȿ���� ������ �����ϴ� �̺�Ʈ �ڵ鷯 �Լ��Դϴ�.
void USettingsOverlay::HandleEffectSoundVolumeChanged(float value)
{
	// GameInstance�� null�̸� ���� �Ҵ��մϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance�� �����ϸ� ȿ���� ������ �����մϴ�.
	if (GameInstance) {
		GameInstance->SetEffectVolume(value);
	}
}

// ���콺 �ΰ����� �����ϴ� �̺�Ʈ �ڵ鷯 �Լ��Դϴ�.
void USettingsOverlay::HandleMouseSensitivitySliderChanged(float value)
{
	// GameInstance�� null�̸� ���� �Ҵ��մϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance�� �����ϸ� ���콺 �ΰ����� �����մϴ�. 
	// ���⼭�� value�� 5�� ���� ���� ���콺 �ΰ����� �����մϴ�.
	if (GameInstance) {
		GameInstance->SetMouseSensitivity(value * 5);
	}
}

// ���ػ� ��ư Ŭ�� �̺�Ʈ �ڵ鷯�Դϴ�.
void USettingsOverlay::OnResolutionHighButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// ������ �����ϸ� ȭ�� �ػ󵵸� 3840x2160���� �����ϰ� �����մϴ�.
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(3840, 2160));
		Settings->ApplySettings(true);
	}
}

// �߰� �ػ� ��ư Ŭ�� �̺�Ʈ �ڵ鷯�Դϴ�.
void USettingsOverlay::OnResolutionMiddleButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// ������ �����ϸ� ȭ�� �ػ󵵸� 1920x1080���� �����ϰ� �����մϴ�.
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		Settings->ApplySettings(true);
	}
}

// ���ػ� ��ư Ŭ�� �̺�Ʈ �ڵ鷯�Դϴ�.
void USettingsOverlay::OnResolutionLowButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// ������ �����ϸ� ȭ�� �ػ󵵸� 1280x720���� �����ϰ� �����մϴ�.
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(1280, 720));
		Settings->ApplySettings(true);
	}
}

// ��ü ȭ�� ��� ��ư Ŭ�� �̺�Ʈ �ڵ鷯�Դϴ�.
void USettingsOverlay::OnFullScreenModeButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// ������ �����ϸ� ��ü ȭ�� ���� �����ϰ� �����մϴ�.
	if (Settings) {
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		Settings->ApplySettings(true);
	}
}


// â ��� ��ư Ŭ�� �̺�Ʈ�� ���� �Լ��Դϴ�.
void USettingsOverlay::OnWindowModeButtonClicked()
{
	// UGameUserSettings�� ����Ͽ� ���� ������ ����
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// Settings�� ��ȿ���� Ȯ��
	if (Settings) {
		// â ���� ���� ����
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		// ���� ����
		Settings->ApplySettings(true);
	}
}

// ������ ��ư Ŭ�� �̺�Ʈ�� ���� �Լ��Դϴ�.
void USettingsOverlay::OnQuitButtonClicked()
{
	// ù ��° �÷��̾��� HUD�� ������ ALobbyLevelHUD�� ĳ����
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// HUD�� ��ȿ�ϴٸ�
	if (HUD) {
		// ���� �������̸� ����
		HUD->HideSettingsOverlay();
	}
	else {
		// HUD�� ���� ��� ���� ��ü�� ������
		UWorld* World = GetWorld();

		// World�� ��ȿ���� üũ
		if (World != nullptr) {
			// �κ� ������ �̵��� �� �̸� ����
			FString mapName = TEXT("/Game/Maps/LobbyLevel");
			// ������ ������ �̵�
			UGameplayStatics::OpenLevel(World, FName(*mapName));
		}
	}
}

// ����ϱ� ��ư Ŭ�� �̺�Ʈ�� ���� �Լ��Դϴ�.
void USettingsOverlay::OnContinueButtonClicked()
{
	// ���� ��ü�� ������
	UWorld* World = GetWorld();

	// World�� ��ȿ���� üũ
	if (World)
	{
		// ù ��° �÷��̾� ��Ʈ�ѷ��� ������
		APlayerController* PlayerController = World->GetFirstPlayerController();

		// PlayerController�� ��ȿ�ϴٸ�
		if (PlayerController)
		{
			// �Ͻ� ���� ���¸� ����
			PlayerController->SetPause(false);
		}

		// Controller�� null�̶�� ù ��° �÷��̾� ��Ʈ�ѷ��� ������ AFantasyPlayerController�� ĳ����
		Controller = Controller == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : Controller;

		// Controller�� ��ȿ�ϴٸ�
		if (Controller) {
			// ĳ���͸� �ٽ� ����
			Controller->RePossess();
			// ���콺 Ŀ�� ����
			Controller->SetShowMouseCursor(false);
		}
	}
	// �� ���������� ���ü��� �������� ����
	this->SetVisibility(ESlateVisibility::Hidden);
}

// ����ϱ� ��ư�� ���߱� ���� �Լ��Դϴ�.
void USettingsOverlay::HideContinueButton()
{
	// ContinueButton�� ���ü��� �������� ����
	ContinueButton->SetVisibility(ESlateVisibility::Hidden);
}
