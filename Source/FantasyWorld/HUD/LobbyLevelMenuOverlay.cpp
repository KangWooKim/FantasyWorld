// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyLevelMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "LobbyLevelHUD.h"
#include "Engine/Texture2D.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"

void ULobbyLevelMenuOverlay::NativeConstruct()
{
	// 부모 클래스의 NativeConstruct 메서드 호출
	Super::NativeConstruct();

	// StartGameButton이 null이 아니면 OnStartGameButtonMenuButtonClicked 함수를 버튼 클릭 이벤트에 연결
	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked);
	}

	// TutorialButton이 null이 아니면 OnTutorialButtonClicked 함수를 버튼 클릭 이벤트에 연결
	if (TutorialButton) {
		TutorialButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnTutorialButtonClicked);
	}

	// SettingsButton이 null이 아니면 OnSettingsButtonClicked 함수를 버튼 클릭 이벤트에 연결
	if (SettingsButton) {
		SettingsButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnSettingsButtonClicked);
	}

	// QuitButton이 null이 아니면 OnQuitButtonClicked 함수를 버튼 클릭 이벤트에 연결
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnQuitButtonClicked);
	}

	// MediaSources 배열의 첫 번째 원소가 있다면 MediaPlayer에 연결하고 재생 시작
	if (MediaSources.Num() > 0) {
		MediaPlayer->OpenSource(MediaSources[0]);
		MediaPlayer->Play();
	}

	// MediaTexture를 MediaPlayer에 연결
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// MediaTexture을 Brush에 설정하고 Image의 Brush를 업데이트
	Brush.SetResourceObject(MediaTexture);
	Image->SetBrush(Brush);
}

void ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked() {
	// 버튼 클릭 소리가 설정되어 있다면 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// HUD 인스턴스를 얻어 SelectMapOverlay를 표시
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSelectMapOverlay();
}

void ULobbyLevelMenuOverlay::OnTutorialButtonClicked() {
	// 버튼 클릭 소리가 설정되어 있다면 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// 현재 세계(World)에 대한 포인터를 얻음
	UWorld* World = GetWorld();
	if (World != nullptr) // 포인터가 유효한지 항상 확인
	{
		FString mapName = TEXT("/Game/Maps/TutorialLevel"); // 여러분의 맵 이름으로 변경
		UGameplayStatics::OpenLevel(World, FName(*mapName)); // 지정한 레벨을 열기
	}
}


// Settings 버튼을 클릭했을 때 호출되는 메서드
void ULobbyLevelMenuOverlay::OnSettingsButtonClicked()
{
	// ButtonClickedSound가 설정되어 있으면 소리를 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// HUD 인스턴스를 얻어서 ShowSettingsOverlay 메서드를 호출하여 설정 오버레이를 표시
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSettingsOverlay();
}

// Quit 버튼을 클릭했을 때 호출되는 메서드
void ULobbyLevelMenuOverlay::OnQuitButtonClicked()
{
	// ButtonClickedSound가 설정되어 있으면 소리를 재생
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// 게임을 종료. EQuitPreference::Quit는 깨끗하게 게임을 종료하는 옵션.
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
