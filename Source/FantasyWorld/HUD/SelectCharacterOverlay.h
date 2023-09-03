// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "SelectCharacterOverlay.generated.h"

class UButton;
class UFileMediaSource;
class USoundBase;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API USelectCharacterOverlay : public UUserWidget
{
	GENERATED_BODY()

public:

	// �⺻ ���� �Լ�. �������̰� �����Ǹ� ȣ��˴ϴ�.
	void NativeConstruct() override;

	// Countess ��ư�� Ŭ������ �� ȣ��� �Լ�
	UFUNCTION()
		void OnCountessButtonClicked();

	// Phase ��ư�� Ŭ������ �� ȣ��� �Լ�
	UFUNCTION()
		void OnPhaseButtonClicked();

	// Quit ��ư�� Ŭ������ �� ȣ��� �Լ�
	UFUNCTION()
		void OnQuitButtonClicked();

	// Countess ��ư�� ���콺�� �÷��� �� ȣ��� �Լ�
	UFUNCTION()
		void OnHoveredCountessButton();

	// Countess ��ư���� ���콺�� ������ �� ȣ��� �Լ�
	UFUNCTION()
		void OnUnHoveredCountessButton();

	// Phase ��ư�� ���콺�� �÷��� �� ȣ��� �Լ�
	UFUNCTION()
		void OnHoverPhaseButton();

	// Phase ��ư���� ���콺�� ������ �� ȣ��� �Լ�
	UFUNCTION()
		void OnUnHoverPhaseButton();

protected:

	// ����

private:

	// Countess ��ư�� ����
	UPROPERTY(meta = (BindWidget))
		UButton* CountessButton;

	// Phase ��ư�� ����
	UPROPERTY(meta = (BindWidget))
		UButton* PhaseButton;

	// Quit ��ư�� ����
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	// ��ư Ŭ�� �� ����� ����
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// �̵� ����� �̵�� �÷��̾�
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MediaPlayer;

	// �̵�� �ҽ� ���
	UPROPERTY(EditAnywhere)
		TArray<UFileMediaSource*> MediaSources;

	// �̵�� �ؽ���
	UPROPERTY(EditAnywhere)
		UMediaTexture* MediaTexture;

	// Slate �귯��. ���⿡ �̵�� �ؽ��ĳ� �ٸ� �ؽ�ó�� ������ �� �ֽ��ϴ�.
	UPROPERTY()
		FSlateBrush Brush;

	// �̹��� ����. �̵� �ؽ�ó�� ǥ���ϴ� �� ���˴ϴ�.
	UPROPERTY(meta = (BindWidget))
		UImage* Image;

};
