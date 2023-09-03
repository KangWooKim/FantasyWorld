// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectCharacterOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "LobbyLevelHUD.h"
#include "Engine/Texture2D.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"

// USelectCharacterOverlay 클래스의 NativeConstruct 함수
// 이 함수는 이 오버레이가 생성될 때 자동으로 호출됩니다.
void USelectCharacterOverlay::NativeConstruct()
{
	// 부모 클래스의 NativeConstruct 함수를 호출
	Super::NativeConstruct();

	// CountessButton이 유효한 경우
	if (CountessButton) {
		// CountessButton의 클릭, 호버, 언호버 이벤트에 대한 콜백 함수를 설정
		CountessButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnCountessButtonClicked);
		CountessButton->OnHovered.AddDynamic(this, &USelectCharacterOverlay::OnHoveredCountessButton);
		CountessButton->OnUnhovered.AddDynamic(this, &USelectCharacterOverlay::OnUnHoveredCountessButton);
	}

	// PhaseButton이 유효한 경우
	if (PhaseButton) {
		// PhaseButton의 클릭, 호버, 언호버 이벤트에 대한 콜백 함수를 설정
		PhaseButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnPhaseButtonClicked);
		PhaseButton->OnHovered.AddDynamic(this, &USelectCharacterOverlay::OnHoverPhaseButton);
		PhaseButton->OnUnhovered.AddDynamic(this, &USelectCharacterOverlay::OnUnHoverPhaseButton);
	}

	// QuitButton이 유효한 경우
	if (QuitButton) {
		// QuitButton의 클릭 이벤트에 대한 콜백 함수를 설정
		QuitButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnQuitButtonClicked);
	}
}

// Countess 버튼을 클릭했을 때의 처리
void USelectCharacterOverlay::OnCountessButtonClicked()
{
	// ButtonClickedSound이 유효하면 사운드를 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// 게임 인스턴스를 얻어온다.
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	// 게임 인스턴스가 유효하면
	if (GameInstance) {
		// 선택된 캐릭터를 Countess로 설정
		GameInstance->SetSelectedCharacter(ECharacterTypes::Countess);
		// 레벨 시작
		GameInstance->StartLevel();
	}
}

// Phase 버튼을 클릭했을 때의 처리
void USelectCharacterOverlay::OnPhaseButtonClicked()
{
	// ButtonClickedSound이 유효하면 사운드를 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// 게임 인스턴스를 얻어온다.
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	// 게임 인스턴스가 유효하면
	if (GameInstance) {
		// 선택된 캐릭터를 Phase로 설정
		GameInstance->SetSelectedCharacter(ECharacterTypes::Phase);
		// 레벨 시작
		GameInstance->StartLevel();
	}
}


// Quit 버튼을 클릭했을 때의 처리 함수
void USelectCharacterOverlay::OnQuitButtonClicked()
{
	// ButtonClickedSound이 유효하면 사운드를 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// 이 오버레이의 가시성을 숨김으로 설정
	this->SetVisibility(ESlateVisibility::Hidden);

	// HUD를 얻어온다.
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// Map 선택 오버레이를 표시
	HUD->ShowSelectMapOverlay();
}

// Countess 버튼 위로 마우스가 올라갔을 때의 처리 함수
void USelectCharacterOverlay::OnHoveredCountessButton()
{
	// MediaSources가 2개 이상 있는 경우
	if (MediaSources.Num() > 1) {
		// 첫 번째 미디어 소스를 열고 재생
		MediaPlayer->OpenSource(MediaSources[0]);
		MediaPlayer->Play();
	}

	// MediaPlayer를 MediaTexture에 설정
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// MediaTexture를 Brush에 설정
	Brush.SetResourceObject(MediaTexture);
	// Brush를 Image 위젯에 설정
	Image->SetBrush(Brush);
}

// Countess 버튼에서 마우스가 벗어났을 때의 처리 함수
void USelectCharacterOverlay::OnUnHoveredCountessButton()
{
	// 빈 브러쉬를 생성
	FSlateBrush EmptyBrush;
	// 브러쉬의 색상을 투명하게 설정
	EmptyBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	// 빈 브러쉬를 Image 위젯에 설정
	Image->SetBrush(EmptyBrush);
}

// Phase 버튼 위로 마우스가 올라갔을 때의 처리 함수
void USelectCharacterOverlay::OnHoverPhaseButton()
{
	// MediaSources가 2개 이상 있는 경우
	if (MediaSources.Num() > 1) {
		// 두 번째 미디어 소스를 열고 재생
		MediaPlayer->OpenSource(MediaSources[1]);
		MediaPlayer->Play();
	}

	// MediaPlayer를 MediaTexture에 설정
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// MediaTexture를 Brush에 설정
	Brush.SetResourceObject(MediaTexture);
	// Brush를 Image 위젯에 설정
	Image->SetBrush(Brush);
}

// Phase 버튼에서 마우스가 벗어났을 때의 처리 함수
void USelectCharacterOverlay::OnUnHoverPhaseButton()
{
	// 빈 브러쉬를 생성
	FSlateBrush EmptyBrush;
	// 브러쉬의 색상을 투명하게 설정
	EmptyBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	// 빈 브러쉬를 Image 위젯에 설정
	Image->SetBrush(EmptyBrush);
}
