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



// USettingsOverlay의 NativeConstruct 함수. 이 함수는 Overlay가 생성될 때 호출되는 함수입니다.
void USettingsOverlay::NativeConstruct()
{
	// 부모 클래스의 NativeConstruct 함수를 호출합니다.
	Super::NativeConstruct();

	// GameInstance와 Controller가 null인지 확인하고, null이면 새로 할당합니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	Controller = Controller == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : Controller;

	// BGMVolume 슬라이더가 존재하면 설정을 초기화하고 이벤트를 바인딩합니다.
	if (BGMVolume) {
		// 슬라이더의 색깔을 녹색으로 설정합니다.
		BGMVolume->WidgetStyle.NormalBarImage.TintColor = FSlateColor(FLinearColor::Green);

		// GameInstance가 존재하면 BGM 볼륨 값을 가져와서 슬라이더에 설정합니다.
		if (GameInstance) {
			BGMVolume->SetValue(GameInstance->GetBGMVolume());
		}
		else {
			// GameInstance가 없다면 기본값 1을 설정합니다.
			BGMVolume->SetValue(1.f);
		}

		// BGMVolume 값이 변경될 때 호출될 함수를 바인딩합니다.
		BGMVolume->OnValueChanged.AddDynamic(this, &USettingsOverlay::HandleBGMVolumeChanged);
	}

	// 이하 코드는 위의 BGMVolume과 비슷한 로직이지만 이펙트 사운드 볼륨을 위한 것입니다.
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

	// 마우스 민감도에 대한 슬라이더 설정
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

	// 고해상도 버튼에 대한 이벤트 바인딩
	if (ResolutionHigh) {
		ResolutionHigh->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionHighButtonClicked);
	}

	// 중간 해상도 버튼에 대한 이벤트 바인딩
	if (ResolutionMiddle) {
		ResolutionMiddle->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionMiddleButtonClicked);
	}

	// 저해상도 버튼에 대한 이벤트 바인딩
	if (ResolutionLow) {
		ResolutionLow->OnClicked.AddDynamic(this, &USettingsOverlay::OnResolutionLowButtonClicked);
	}

	// 전체화면 모드 버튼에 대한 이벤트 바인딩
	if (FullScreenMode) {
		FullScreenMode->OnClicked.AddDynamic(this, &USettingsOverlay::OnFullScreenModeButtonClicked);
	}

	// 윈도우 모드 버튼에 대한 이벤트 바인딩
	if (WindowMode) {
		WindowMode->OnClicked.AddDynamic(this, &USettingsOverlay::OnWindowModeButtonClicked);
	}

	// 종료 버튼에 대한 이벤트 바인딩
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnQuitButtonClicked);
	}

	// 계속 버튼에 대한 이벤트 바인딩
	if (ContinueButton) {
		ContinueButton->OnClicked.AddDynamic(this, &USettingsOverlay::OnContinueButtonClicked);
	}
}


// BGM 볼륨을 변경하는 이벤트 핸들러 함수입니다.
void USettingsOverlay::HandleBGMVolumeChanged(float value)
{
	// GameInstance가 null이면 새로 할당합니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance가 존재하면 BGM 볼륨을 설정합니다.
	if (GameInstance) {
		GameInstance->SetBGMVolume(value);
	}
}

// 효과음 볼륨을 변경하는 이벤트 핸들러 함수입니다.
void USettingsOverlay::HandleEffectSoundVolumeChanged(float value)
{
	// GameInstance가 null이면 새로 할당합니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance가 존재하면 효과음 볼륨을 설정합니다.
	if (GameInstance) {
		GameInstance->SetEffectVolume(value);
	}
}

// 마우스 민감도를 변경하는 이벤트 핸들러 함수입니다.
void USettingsOverlay::HandleMouseSensitivitySliderChanged(float value)
{
	// GameInstance가 null이면 새로 할당합니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance가 존재하면 마우스 민감도를 설정합니다. 
	// 여기서는 value에 5를 곱해 실제 마우스 민감도를 설정합니다.
	if (GameInstance) {
		GameInstance->SetMouseSensitivity(value * 5);
	}
}

// 고해상도 버튼 클릭 이벤트 핸들러입니다.
void USettingsOverlay::OnResolutionHighButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// 설정이 존재하면 화면 해상도를 3840x2160으로 설정하고 적용합니다.
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(3840, 2160));
		Settings->ApplySettings(true);
	}
}

// 중간 해상도 버튼 클릭 이벤트 핸들러입니다.
void USettingsOverlay::OnResolutionMiddleButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// 설정이 존재하면 화면 해상도를 1920x1080으로 설정하고 적용합니다.
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(1920, 1080));
		Settings->ApplySettings(true);
	}
}

// 저해상도 버튼 클릭 이벤트 핸들러입니다.
void USettingsOverlay::OnResolutionLowButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// 설정이 존재하면 화면 해상도를 1280x720으로 설정하고 적용합니다.
	if (Settings) {
		Settings->SetScreenResolution(FIntPoint(1280, 720));
		Settings->ApplySettings(true);
	}
}

// 전체 화면 모드 버튼 클릭 이벤트 핸들러입니다.
void USettingsOverlay::OnFullScreenModeButtonClicked()
{
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// 설정이 존재하면 전체 화면 모드로 설정하고 적용합니다.
	if (Settings) {
		Settings->SetFullscreenMode(EWindowMode::Fullscreen);
		Settings->ApplySettings(true);
	}
}


// 창 모드 버튼 클릭 이벤트를 위한 함수입니다.
void USettingsOverlay::OnWindowModeButtonClicked()
{
	// UGameUserSettings를 사용하여 게임 설정에 접근
	UGameUserSettings* Settings = GEngine->GameUserSettings;

	// Settings가 유효한지 확인
	if (Settings) {
		// 창 모드로 설정 변경
		Settings->SetFullscreenMode(EWindowMode::Windowed);
		// 설정 적용
		Settings->ApplySettings(true);
	}
}

// 나가기 버튼 클릭 이벤트를 위한 함수입니다.
void USettingsOverlay::OnQuitButtonClicked()
{
	// 첫 번째 플레이어의 HUD를 가져와 ALobbyLevelHUD로 캐스팅
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	// HUD가 유효하다면
	if (HUD) {
		// 설정 오버레이를 숨김
		HUD->HideSettingsOverlay();
	}
	else {
		// HUD가 없을 경우 월드 객체를 가져옴
		UWorld* World = GetWorld();

		// World가 유효한지 체크
		if (World != nullptr) {
			// 로비 레벨로 이동할 맵 이름 설정
			FString mapName = TEXT("/Game/Maps/LobbyLevel");
			// 지정된 레벨로 이동
			UGameplayStatics::OpenLevel(World, FName(*mapName));
		}
	}
}

// 계속하기 버튼 클릭 이벤트를 위한 함수입니다.
void USettingsOverlay::OnContinueButtonClicked()
{
	// 월드 객체를 가져옴
	UWorld* World = GetWorld();

	// World가 유효한지 체크
	if (World)
	{
		// 첫 번째 플레이어 컨트롤러를 가져옴
		APlayerController* PlayerController = World->GetFirstPlayerController();

		// PlayerController가 유효하다면
		if (PlayerController)
		{
			// 일시 중지 상태를 해제
			PlayerController->SetPause(false);
		}

		// Controller가 null이라면 첫 번째 플레이어 컨트롤러를 가져와 AFantasyPlayerController로 캐스팅
		Controller = Controller == nullptr ? Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController()) : Controller;

		// Controller가 유효하다면
		if (Controller) {
			// 캐릭터를 다시 조종
			Controller->RePossess();
			// 마우스 커서 숨김
			Controller->SetShowMouseCursor(false);
		}
	}
	// 이 오버레이의 가시성을 숨김으로 설정
	this->SetVisibility(ESlateVisibility::Hidden);
}

// 계속하기 버튼을 감추기 위한 함수입니다.
void USettingsOverlay::HideContinueButton()
{
	// ContinueButton의 가시성을 숨김으로 설정
	ContinueButton->SetVisibility(ESlateVisibility::Hidden);
}
