// Fill out your copyright notice in the Description page of Project Settings.


#include "DieMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/TextBlock.h"

// NativeConstruct는 UserWidget 클래스에서 오버라이딩한 함수로, UI가 구성되었을 때 호출됩니다.
void UDieMenuOverlay::NativeConstruct()
{
	Super::NativeConstruct();  // 부모 클래스의 NativeConstruct 호출

	// 'ReturnToMenu' 버튼이 nullptr이 아니라면 클릭 이벤트에 함수를 바인딩합니다.
	if (ReturnToMenu) {
		ReturnToMenu->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnReturnToMenuButtonClicked);
	}

	// 'RestartLevel' 버튼이 nullptr이 아니라면 클릭 이벤트에 함수를 바인딩합니다.
	if (RestartLevel) {
		RestartLevel->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnRestartLevelButtonClicked);
	}
}

// 게임에서 죽은 후 나타날 텍스트를 업데이트합니다.
// SlayedEnemy: 플레이어가 죽인 적의 수, LeftEnemy: 아직 남아있는 적의 수
void UDieMenuOverlay::UpdateTextBlock(int32 SlayedEnemy, int32 LeftEnemy)
{
	FString CombinedString = FString::Printf(TEXT("You cut %d enemies, but %d enemies still remain in the world."), SlayedEnemy, LeftEnemy);
	FText NewText = FText::FromString(CombinedString);
	Text->SetText(NewText);  // 텍스트 블록에 새로운 텍스트를 설정합니다.
}

// 'ReturnToMenu' 버튼이 클릭되면 호출되는 함수입니다.
void UDieMenuOverlay::OnReturnToMenuButtonClicked()
{
	// 버튼이 클릭될 때 사운드를 재생합니다.
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UWorld* World = GetWorld();  // UWorld 포인터를 가져옵니다.
	// 포인터가 유효한지 확인합니다.
	if (World != nullptr)
	{
		FString mapName = TEXT("/Game/Maps/LobbyLevel");  // 로비 레벨의 이름입니다.
		UGameplayStatics::OpenLevel(World, FName(*mapName));  // 지정된 레벨을 여는 함수를 호출합니다.
	}
}

// 'RestartLevel' 버튼이 클릭되면 호출되는 함수입니다.
void UDieMenuOverlay::OnRestartLevelButtonClicked()
{
	// 버튼이 클릭될 때 사운드를 재생합니다.
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());  // 게임 인스턴스를 가져옵니다.
	FString mapName = GameInstance->GetSelectedMap();  // 게임 인스턴스에서 선택된 맵의 이름을 가져옵니다.
	UGameplayStatics::OpenLevel(this, FName(*mapName));  // 지정된 레벨을 열어 재시작합니다.
}
