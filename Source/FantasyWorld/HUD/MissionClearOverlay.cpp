// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionClearOverlay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// NativeConstruct 함수는 UI 위젯이 생성될 때 호출됩니다.
void UMissionClearOverlay::NativeConstruct()
{
	// QuitButton이 유효하다면, 클릭 이벤트에 OnQuitButtonClicked 함수를 연결합니다.
	if (QuitButton) {
		QuitButton->OnClicked.AddDynamic(this, &UMissionClearOverlay::OnQuitButtonClicked);
	}
}

// QuitButton이 클릭되었을 때 호출되는 함수입니다.
void UMissionClearOverlay::OnQuitButtonClicked()
{
	// 버튼 클릭 사운드가 설정되어 있다면 사운드를 재생합니다.
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	// 현재 월드(World)의 포인터를 얻습니다.
	UWorld* World = GetWorld();

	// 월드 포인터가 유효한지 확인합니다.
	if (World != nullptr)
	{
		// 이동할 맵의 이름을 지정합니다.
		FString mapName = TEXT("/Game/Maps/LobbyLevel");

		// 지정된 맵으로 이동합니다.
		UGameplayStatics::OpenLevel(World, FName(*mapName));
	}
}

// SlayedEnemy의 값을 받아 텍스트 블록을 업데이트하는 함수입니다.
void UMissionClearOverlay::UpdateTextBlock(int32 SlayedEnemy)
{
	// 적을 몇 마리 처치했는지 나타내는 문자열을 생성합니다.
	FString CombinedString = FString::Printf(TEXT("You saved the world by cutting down %d enemies"), SlayedEnemy);

	// 생성한 문자열을 FText 형식으로 변환합니다.
	FText NewText = FText::FromString(CombinedString);

	// 텍스트 블록의 내용을 새로운 텍스트로 업데이트합니다.
	Text->SetText(NewText);
}
