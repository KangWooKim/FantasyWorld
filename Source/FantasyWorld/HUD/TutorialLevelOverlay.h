// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MediaAssets/Public/MediaPlayer.h"
#include "MediaAssets/Public/MediaTexture.h"
#include "Runtime/UMG/Public/Components/Image.h"
#include "TutorialLevelOverlay.generated.h"

class UButton;
class UFileMediaSource;
class USoundBase;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API UTutorialLevelOverlay : public UUserWidget
{
	GENERATED_BODY()

		// Ŭ������ public ����
public:

	// UUserWidget���� ��ӹ��� NativeConstruct �Լ��� �������̵���. 
	// �� �Լ��� ������ ������ �� �� ���� ȣ���.
	virtual void NativeConstruct() override;

	// OkButton�� Ŭ���Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnOkButtonClicked();

	// QuitButton�� Ŭ���Ǹ� ȣ��Ǵ� �Լ�
	UFUNCTION()
		void OnQuitButtonClicked();

	// ���� ������ ����ϴ� �Լ�
	UFUNCTION()
		void PlayNextVideo();

	// ������ ���ü��� Ȱ��ȭ�ϴ� �Լ�
	UFUNCTION()
		void VisibilityEnabled();

	// Ŭ������ protected ����
protected:

	// Ŭ������ private ����
private:

	// Media Player ������Ʈ. ������ ����ϱ� ���� ����.
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MediaPlayer;

	// ����� �̵�� ������ �迭
	UPROPERTY(EditAnywhere)
		TArray<UFileMediaSource*> MediaSources;

	// �̵�� �ؽ���. �̵�� �÷��̾�� ��µ� �ؽ���
	UPROPERTY(EditAnywhere)
		UMediaTexture* MediaTexture;

	// Slate �귯��. UI�� �׸��� �� ����.
	UPROPERTY()
		FSlateBrush Brush;

	// �̹��� ����. �귯�÷� ������.
	UPROPERTY(meta = (BindWidget))
		UImage* Image;

	// ���� ������� ������ �ε���
	UPROPERTY(EditAnywhere)
		int32 CurrentVideoIndex = 0;

	// Ok ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* OkButton;

	// ��ư Ŭ�� ����
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// Quit ��ư ����
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	
};
