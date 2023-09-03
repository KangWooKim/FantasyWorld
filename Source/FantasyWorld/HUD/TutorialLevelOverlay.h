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

		// 클래스의 public 섹션
public:

	// UUserWidget에서 상속받은 NativeConstruct 함수를 오버라이딩함. 
	// 이 함수는 위젯이 생성될 때 한 번만 호출됨.
	virtual void NativeConstruct() override;

	// OkButton이 클릭되면 호출되는 함수
	UFUNCTION()
		void OnOkButtonClicked();

	// QuitButton이 클릭되면 호출되는 함수
	UFUNCTION()
		void OnQuitButtonClicked();

	// 다음 비디오를 재생하는 함수
	UFUNCTION()
		void PlayNextVideo();

	// 위젯의 가시성을 활성화하는 함수
	UFUNCTION()
		void VisibilityEnabled();

	// 클래스의 protected 섹션
protected:

	// 클래스의 private 섹션
private:

	// Media Player 오브젝트. 비디오를 재생하기 위해 사용됨.
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MediaPlayer;

	// 재생할 미디어 파일의 배열
	UPROPERTY(EditAnywhere)
		TArray<UFileMediaSource*> MediaSources;

	// 미디어 텍스쳐. 미디어 플레이어에서 출력될 텍스쳐
	UPROPERTY(EditAnywhere)
		UMediaTexture* MediaTexture;

	// Slate 브러시. UI를 그리는 데 사용됨.
	UPROPERTY()
		FSlateBrush Brush;

	// 이미지 위젯. 브러시로 설정됨.
	UPROPERTY(meta = (BindWidget))
		UImage* Image;

	// 현재 재생중인 비디오의 인덱스
	UPROPERTY(EditAnywhere)
		int32 CurrentVideoIndex = 0;

	// Ok 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* OkButton;

	// 버튼 클릭 사운드
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// Quit 버튼 위젯
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	
};
