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
	// �θ� Ŭ������ NativeConstruct �޼��� ȣ��
	Super::NativeConstruct();

	// StartGameButton�� null�� �ƴϸ� OnStartGameButtonMenuButtonClicked �Լ��� ��ư Ŭ�� �̺�Ʈ�� ����
	if (StartGameButton) {
		StartGameButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked);
	}

	// TutorialButton�� null�� �ƴϸ� OnTutorialButtonClicked �Լ��� ��ư Ŭ�� �̺�Ʈ�� ����
	if (TutorialButton) {
		TutorialButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnTutorialButtonClicked);
	}

	// SettingsButton�� null�� �ƴϸ� OnSettingsButtonClicked �Լ��� ��ư Ŭ�� �̺�Ʈ�� ����
	if (SettingsButton) {
		SettingsButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnSettingsButtonClicked);
	}

	// QuitButton�� null�� �ƴϸ� OnQuitButtonClicked �Լ��� ��ư Ŭ�� �̺�Ʈ�� ����
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &ULobbyLevelMenuOverlay::OnQuitButtonClicked);
	}

	// MediaSources �迭�� ù ��° ���Ұ� �ִٸ� MediaPlayer�� �����ϰ� ��� ����
	if (MediaSources.Num() > 0) {
		MediaPlayer->OpenSource(MediaSources[0]);
		MediaPlayer->Play();
	}

	// MediaTexture�� MediaPlayer�� ����
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// MediaTexture�� Brush�� �����ϰ� Image�� Brush�� ������Ʈ
	Brush.SetResourceObject(MediaTexture);
	Image->SetBrush(Brush);
}

void ULobbyLevelMenuOverlay::OnStartGameButtonMenuButtonClicked() {
	// ��ư Ŭ�� �Ҹ��� �����Ǿ� �ִٸ� ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// HUD �ν��Ͻ��� ��� SelectMapOverlay�� ǥ��
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSelectMapOverlay();
}

void ULobbyLevelMenuOverlay::OnTutorialButtonClicked() {
	// ��ư Ŭ�� �Ҹ��� �����Ǿ� �ִٸ� ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// ���� ����(World)�� ���� �����͸� ����
	UWorld* World = GetWorld();
	if (World != nullptr) // �����Ͱ� ��ȿ���� �׻� Ȯ��
	{
		FString mapName = TEXT("/Game/Maps/TutorialLevel"); // �������� �� �̸����� ����
		UGameplayStatics::OpenLevel(World, FName(*mapName)); // ������ ������ ����
	}
}


// Settings ��ư�� Ŭ������ �� ȣ��Ǵ� �޼���
void ULobbyLevelMenuOverlay::OnSettingsButtonClicked()
{
	// ButtonClickedSound�� �����Ǿ� ������ �Ҹ��� ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// HUD �ν��Ͻ��� �� ShowSettingsOverlay �޼��带 ȣ���Ͽ� ���� �������̸� ǥ��
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->ShowSettingsOverlay();
}

// Quit ��ư�� Ŭ������ �� ȣ��Ǵ� �޼���
void ULobbyLevelMenuOverlay::OnQuitButtonClicked()
{
	// ButtonClickedSound�� �����Ǿ� ������ �Ҹ��� ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// ������ ����. EQuitPreference::Quit�� �����ϰ� ������ �����ϴ� �ɼ�.
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
