// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "LobbyLevelMenuOverlay.generated.h"

class UButton;
class USoundBase;
class UFileMediaSource;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ULobbyLevelMenuOverlay : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// NativeConstruct()�� UI�� ������ �� ȣ��Ǵ� �Լ�
	void NativeConstruct();

	// ���� ���� ��ư�� Ŭ���Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnStartGameButtonMenuButtonClicked();

	// Ʃ�丮�� ��ư�� Ŭ���Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnTutorialButtonClicked();

	// ���� ��ư�� Ŭ���Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnSettingsButtonClicked();

	// ���� ��ư�� Ŭ���Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnQuitButtonClicked();

protected:

private:

	// ���� ���� ��ư�� ���� ����
	UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;

	// Ʃ�丮�� ��ư�� ���� ����
	UPROPERTY(meta = (BindWidget))
		UButton* TutorialButton;

	// ���� ��ư�� ���� ����
	UPROPERTY(meta = (BindWidget))
		UButton* SettingsButton;

	// ���� ��ư�� ���� ����
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	// ��ư Ŭ�� �� ����� �Ҹ�
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// �̵�� �÷��̾� ��ü
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MediaPlayer;

	// �̵�� �ҽ� ���
	UPROPERTY(EditAnywhere)
		TArray<UFileMediaSource*> MediaSources;

	// �̵�� �ؽ��� ��ü
	UPROPERTY(EditAnywhere)
		UMediaTexture* MediaTexture;

	// ������Ʈ �귯�� ��ü (UI�� �ؽ�ó�� ǥ���ϱ� ���� �귯��)
	UPROPERTY()
		FSlateBrush Brush;

	// �̹��� ��ü�� ���� ����
	UPROPERTY(meta = (BindWidget))
		UImage* Image;

};
