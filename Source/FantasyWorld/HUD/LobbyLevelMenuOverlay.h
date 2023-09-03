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

	// NativeConstruct()는 UI가 구성될 때 호출되는 함수
	void NativeConstruct();

	// 게임 시작 버튼이 클릭되면 호출되는 함수
	UFUNCTION()
		void OnStartGameButtonMenuButtonClicked();

	// 튜토리얼 버튼이 클릭되면 호출되는 함수
	UFUNCTION()
		void OnTutorialButtonClicked();

	// 설정 버튼이 클릭되면 호출되는 함수
	UFUNCTION()
		void OnSettingsButtonClicked();

	// 종료 버튼이 클릭되면 호출되는 함수
	UFUNCTION()
		void OnQuitButtonClicked();

protected:

private:

	// 게임 시작 버튼에 대한 참조
	UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;

	// 튜토리얼 버튼에 대한 참조
	UPROPERTY(meta = (BindWidget))
		UButton* TutorialButton;

	// 설정 버튼에 대한 참조
	UPROPERTY(meta = (BindWidget))
		UButton* SettingsButton;

	// 종료 버튼에 대한 참조
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	// 버튼 클릭 시 재생될 소리
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// 미디어 플레이어 객체
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MediaPlayer;

	// 미디어 소스 목록
	UPROPERTY(EditAnywhere)
		TArray<UFileMediaSource*> MediaSources;

	// 미디어 텍스쳐 객체
	UPROPERTY(EditAnywhere)
		UMediaTexture* MediaTexture;

	// 슬레이트 브러시 객체 (UI에 텍스처를 표시하기 위한 브러시)
	UPROPERTY()
		FSlateBrush Brush;

	// 이미지 객체에 대한 참조
	UPROPERTY(meta = (BindWidget))
		UImage* Image;

};
