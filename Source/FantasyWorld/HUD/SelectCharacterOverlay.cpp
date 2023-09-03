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

// USelectCharacterOverlay Ŭ������ NativeConstruct �Լ�
// �� �Լ��� �� �������̰� ������ �� �ڵ����� ȣ��˴ϴ�.
void USelectCharacterOverlay::NativeConstruct()
{
	// �θ� Ŭ������ NativeConstruct �Լ��� ȣ��
	Super::NativeConstruct();

	// CountessButton�� ��ȿ�� ���
	if (CountessButton) {
		// CountessButton�� Ŭ��, ȣ��, ��ȣ�� �̺�Ʈ�� ���� �ݹ� �Լ��� ����
		CountessButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnCountessButtonClicked);
		CountessButton->OnHovered.AddDynamic(this, &USelectCharacterOverlay::OnHoveredCountessButton);
		CountessButton->OnUnhovered.AddDynamic(this, &USelectCharacterOverlay::OnUnHoveredCountessButton);
	}

	// PhaseButton�� ��ȿ�� ���
	if (PhaseButton) {
		// PhaseButton�� Ŭ��, ȣ��, ��ȣ�� �̺�Ʈ�� ���� �ݹ� �Լ��� ����
		PhaseButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnPhaseButtonClicked);
		PhaseButton->OnHovered.AddDynamic(this, &USelectCharacterOverlay::OnHoverPhaseButton);
		PhaseButton->OnUnhovered.AddDynamic(this, &USelectCharacterOverlay::OnUnHoverPhaseButton);
	}

	// QuitButton�� ��ȿ�� ���
	if (QuitButton) {
		// QuitButton�� Ŭ�� �̺�Ʈ�� ���� �ݹ� �Լ��� ����
		QuitButton->OnClicked.AddDynamic(this, &USelectCharacterOverlay::OnQuitButtonClicked);
	}
}

// Countess ��ư�� Ŭ������ ���� ó��
void USelectCharacterOverlay::OnCountessButtonClicked()
{
	// ButtonClickedSound�� ��ȿ�ϸ� ���带 ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// ���� �ν��Ͻ��� ���´�.
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	// ���� �ν��Ͻ��� ��ȿ�ϸ�
	if (GameInstance) {
		// ���õ� ĳ���͸� Countess�� ����
		GameInstance->SetSelectedCharacter(ECharacterTypes::Countess);
		// ���� ����
		GameInstance->StartLevel();
	}
}

// Phase ��ư�� Ŭ������ ���� ó��
void USelectCharacterOverlay::OnPhaseButtonClicked()
{
	// ButtonClickedSound�� ��ȿ�ϸ� ���带 ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// ���� �ν��Ͻ��� ���´�.
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	// ���� �ν��Ͻ��� ��ȿ�ϸ�
	if (GameInstance) {
		// ���õ� ĳ���͸� Phase�� ����
		GameInstance->SetSelectedCharacter(ECharacterTypes::Phase);
		// ���� ����
		GameInstance->StartLevel();
	}
}


// Quit ��ư�� Ŭ������ ���� ó�� �Լ�
void USelectCharacterOverlay::OnQuitButtonClicked()
{
	// ButtonClickedSound�� ��ȿ�ϸ� ���带 ���
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// �� ���������� ���ü��� �������� ����
	this->SetVisibility(ESlateVisibility::Hidden);

	// HUD�� ���´�.
	ALobbyLevelHUD* HUD = Cast<ALobbyLevelHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	// Map ���� �������̸� ǥ��
	HUD->ShowSelectMapOverlay();
}

// Countess ��ư ���� ���콺�� �ö��� ���� ó�� �Լ�
void USelectCharacterOverlay::OnHoveredCountessButton()
{
	// MediaSources�� 2�� �̻� �ִ� ���
	if (MediaSources.Num() > 1) {
		// ù ��° �̵�� �ҽ��� ���� ���
		MediaPlayer->OpenSource(MediaSources[0]);
		MediaPlayer->Play();
	}

	// MediaPlayer�� MediaTexture�� ����
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// MediaTexture�� Brush�� ����
	Brush.SetResourceObject(MediaTexture);
	// Brush�� Image ������ ����
	Image->SetBrush(Brush);
}

// Countess ��ư���� ���콺�� ����� ���� ó�� �Լ�
void USelectCharacterOverlay::OnUnHoveredCountessButton()
{
	// �� �귯���� ����
	FSlateBrush EmptyBrush;
	// �귯���� ������ �����ϰ� ����
	EmptyBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	// �� �귯���� Image ������ ����
	Image->SetBrush(EmptyBrush);
}

// Phase ��ư ���� ���콺�� �ö��� ���� ó�� �Լ�
void USelectCharacterOverlay::OnHoverPhaseButton()
{
	// MediaSources�� 2�� �̻� �ִ� ���
	if (MediaSources.Num() > 1) {
		// �� ��° �̵�� �ҽ��� ���� ���
		MediaPlayer->OpenSource(MediaSources[1]);
		MediaPlayer->Play();
	}

	// MediaPlayer�� MediaTexture�� ����
	MediaTexture->SetMediaPlayer(MediaPlayer);

	// MediaTexture�� Brush�� ����
	Brush.SetResourceObject(MediaTexture);
	// Brush�� Image ������ ����
	Image->SetBrush(Brush);
}

// Phase ��ư���� ���콺�� ����� ���� ó�� �Լ�
void USelectCharacterOverlay::OnUnHoverPhaseButton()
{
	// �� �귯���� ����
	FSlateBrush EmptyBrush;
	// �귯���� ������ �����ϰ� ����
	EmptyBrush.TintColor = FSlateColor(FLinearColor(0, 0, 0, 0));
	// �� �귯���� Image ������ ����
	Image->SetBrush(EmptyBrush);
}
