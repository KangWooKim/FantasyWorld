// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionClearOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// NativeConstruct �Լ��� UI ������ ������ �� ȣ��˴ϴ�.
void UMissionClearOverlay::NativeConstruct()
{
	// QuitButton�� ��ȿ�ϴٸ�, Ŭ�� �̺�Ʈ�� OnQuitButtonClicked �Լ��� �����մϴ�.
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &UMissionClearOverlay::OnQuitButtonClicked);
	}
}

// QuitButton�� Ŭ���Ǿ��� �� ȣ��Ǵ� �Լ��Դϴ�.
void UMissionClearOverlay::OnQuitButtonClicked()
{
	// ��ư Ŭ�� ���尡 �����Ǿ� �ִٸ� ���带 ����մϴ�.
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// ���� ����(World)�� �����͸� ����ϴ�.
	UWorld* World = GetWorld();

	// ���� �����Ͱ� ��ȿ���� Ȯ���մϴ�.
	if (World != nullptr)
	{
		// �̵��� ���� �̸��� �����մϴ�.
		FString mapName = TEXT("/Game/Maps/LobbyLevel");

		// ������ ������ �̵��մϴ�.
		UGameplayStatics::OpenLevel(World, FName(*mapName));
	}
}

// SlayedEnemy�� ���� �޾� �ؽ�Ʈ ����� ������Ʈ�ϴ� �Լ��Դϴ�.
void UMissionClearOverlay::UpdateTextBlock(int32 SlayedEnemy)
{
	// ���� �� ���� óġ�ߴ��� ��Ÿ���� ���ڿ��� �����մϴ�.
	FString CombinedString = FString::Printf(TEXT("You saved the world by cutting down %d enemies"), SlayedEnemy);

	// ������ ���ڿ��� FText �������� ��ȯ�մϴ�.
	FText NewText = FText::FromString(CombinedString);

	// �ؽ�Ʈ ����� ������ ���ο� �ؽ�Ʈ�� ������Ʈ�մϴ�.
	Text->SetText(NewText);
}
