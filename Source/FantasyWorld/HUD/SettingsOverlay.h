// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsOverlay.generated.h"

class UMyGameInstance;
class USlider;
class UButton;
class AFantasyPlayerController;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API USettingsOverlay : public UUserWidget
{
	GENERATED_BODY()
	
		// 공개(public) 메서드와 변수를 정의하는 섹션
public:
	// NativeConstruct는 클래스가 생성되었을 때 호출되는 함수
	void NativeConstruct();

	// BGM 볼륨이 변경될 때 호출되는 함수, value는 변경된 볼륨 값
	UFUNCTION()
		void HandleBGMVolumeChanged(float value);

	// 효과음 볼륨이 변경될 때 호출되는 함수, value는 변경된 볼륨 값
	UFUNCTION()
		void HandleEffectSoundVolumeChanged(float value);

	// 마우스 감도가 변경될 때 호출되는 함수, value는 변경된 마우스 감도 값
	UFUNCTION()
		void HandleMouseSensitivitySliderChanged(float value);

	// 'Quit' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnQuitButtonClicked();

	// 'Continue' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnContinueButtonClicked();

	// 'Continue' 버튼을 숨기는 함수
	UFUNCTION()
		void HideContinueButton();

	// 'High Resolution' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnResolutionHighButtonClicked();

	// 'Middle Resolution' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnResolutionMiddleButtonClicked();

	// 'Low Resolution' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnResolutionLowButtonClicked();

	// 'Full Screen Mode' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnFullScreenModeButtonClicked();

	// 'Window Mode' 버튼이 클릭될 때 호출되는 함수
	UFUNCTION()
		void OnWindowModeButtonClicked();

	// BGM 볼륨을 조절하는 슬라이더 위젯
	UPROPERTY(meta = (BindWidget))
		USlider* BGMVolume;

	// 효과음 볼륨을 조절하는 슬라이더 위젯
	UPROPERTY(meta = (BindWidget))
		USlider* EffectSoundVolume;

	// 마우스 감도를 조절하는 슬라이더 위젯
	UPROPERTY(meta = (BindWidget))
		USlider* MouseSensitivitySlider;

	// protected 메서드와 변수를 정의하는 섹션
protected:


	// private 메서드와 변수를 정의하는 섹션
private:
	// 플레이어 컨트롤러의 인스턴스
	UPROPERTY()
		AFantasyPlayerController* Controller;

	// 게임 인스턴스
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// 해상도를 'High'로 설정하는 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* ResolutionHigh;

	// 해상도를 'Middle'로 설정하는 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* ResolutionMiddle;

	// 해상도를 'Low'로 설정하는 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* ResolutionLow;

	// 화면을 전체화면 모드로 설정하는 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* FullScreenMode;

	// 화면을 윈도우 모드로 설정하는 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* WindowMode;

	// 게임을 계속하는 'Continue' 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton;

	// 게임에서 나가는 'Quit' 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

};
