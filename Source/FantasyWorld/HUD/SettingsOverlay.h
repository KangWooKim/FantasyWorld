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
	
		// ����(public) �޼���� ������ �����ϴ� ����
public:
	// NativeConstruct�� Ŭ������ �����Ǿ��� �� ȣ��Ǵ� �Լ�
	void NativeConstruct();

	// BGM ������ ����� �� ȣ��Ǵ� �Լ�, value�� ����� ���� ��
	UFUNCTION()
		void HandleBGMVolumeChanged(float value);

	// ȿ���� ������ ����� �� ȣ��Ǵ� �Լ�, value�� ����� ���� ��
	UFUNCTION()
		void HandleEffectSoundVolumeChanged(float value);

	// ���콺 ������ ����� �� ȣ��Ǵ� �Լ�, value�� ����� ���콺 ���� ��
	UFUNCTION()
		void HandleMouseSensitivitySliderChanged(float value);

	// 'Quit' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnQuitButtonClicked();

	// 'Continue' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnContinueButtonClicked();

	// 'Continue' ��ư�� ����� �Լ�
	UFUNCTION()
		void HideContinueButton();

	// 'High Resolution' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnResolutionHighButtonClicked();

	// 'Middle Resolution' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnResolutionMiddleButtonClicked();

	// 'Low Resolution' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnResolutionLowButtonClicked();

	// 'Full Screen Mode' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnFullScreenModeButtonClicked();

	// 'Window Mode' ��ư�� Ŭ���� �� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnWindowModeButtonClicked();

	// BGM ������ �����ϴ� �����̴� ����
	UPROPERTY(meta = (BindWidget))
		USlider* BGMVolume;

	// ȿ���� ������ �����ϴ� �����̴� ����
	UPROPERTY(meta = (BindWidget))
		USlider* EffectSoundVolume;

	// ���콺 ������ �����ϴ� �����̴� ����
	UPROPERTY(meta = (BindWidget))
		USlider* MouseSensitivitySlider;

	// protected �޼���� ������ �����ϴ� ����
protected:


	// private �޼���� ������ �����ϴ� ����
private:
	// �÷��̾� ��Ʈ�ѷ��� �ν��Ͻ�
	UPROPERTY()
		AFantasyPlayerController* Controller;

	// ���� �ν��Ͻ�
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// �ػ󵵸� 'High'�� �����ϴ� ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* ResolutionHigh;

	// �ػ󵵸� 'Middle'�� �����ϴ� ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* ResolutionMiddle;

	// �ػ󵵸� 'Low'�� �����ϴ� ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* ResolutionLow;

	// ȭ���� ��üȭ�� ���� �����ϴ� ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* FullScreenMode;

	// ȭ���� ������ ���� �����ϴ� ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* WindowMode;

	// ������ ����ϴ� 'Continue' ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* ContinueButton;

	// ���ӿ��� ������ 'Quit' ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

};
