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

	// 기본 생성 함수. 오버레이가 생성되면 호출됩니다.
	void NativeConstruct() override;

	// Countess 버튼을 클릭했을 때 호출될 함수
	UFUNCTION()
		void OnCountessButtonClicked();

	// Phase 버튼을 클릭했을 때 호출될 함수
	UFUNCTION()
		void OnPhaseButtonClicked();

	// Quit 버튼을 클릭했을 때 호출될 함수
	UFUNCTION()
		void OnQuitButtonClicked();

	// Countess 버튼에 마우스를 올렸을 때 호출될 함수
	UFUNCTION()
		void OnHoveredCountessButton();

	// Countess 버튼에서 마우스를 내렸을 때 호출될 함수
	UFUNCTION()
		void OnUnHoveredCountessButton();

	// Phase 버튼에 마우스를 올렸을 때 호출될 함수
	UFUNCTION()
		void OnHoverPhaseButton();

	// Phase 버튼에서 마우스를 내렸을 때 호출될 함수
	UFUNCTION()
		void OnUnHoverPhaseButton();

protected:

	// 생략

private:

	// Countess 버튼의 참조
	UPROPERTY(meta = (BindWidget))
		UButton* CountessButton;

	// Phase 버튼의 참조
	UPROPERTY(meta = (BindWidget))
		UButton* PhaseButton;

	// Quit 버튼의 참조
	UPROPERTY(meta = (BindWidget))
		UButton* QuitButton;

	// 버튼 클릭 시 재생될 사운드
	UPROPERTY(EditAnywhere)
		USoundBase* ButtonClickedSound;

	// 미디어를 재생할 미디어 플레이어
	UPROPERTY(EditAnywhere)
		UMediaPlayer* MediaPlayer;

	// 미디어 소스 목록
	UPROPERTY(EditAnywhere)
		TArray<UFileMediaSource*> MediaSources;

	// 미디어 텍스쳐
	UPROPERTY(EditAnywhere)
		UMediaTexture* MediaTexture;

	// Slate 브러쉬. 여기에 미디어 텍스쳐나 다른 텍스처를 설정할 수 있습니다.
	UPROPERTY()
		FSlateBrush Brush;

	// 이미지 위젯. 미디어나 텍스처를 표시하는 데 사용됩니다.
	UPROPERTY(meta = (BindWidget))
		UImage* Image;

};
