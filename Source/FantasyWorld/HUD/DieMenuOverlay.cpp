// Fill out your copyright notice in the Description page of Project Settings.


#include "DieMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/TextBlock.h"

void UDieMenuOverlay::NativeConstruct()
{
	Super::NativeConstruct();


	if (ReturnToMenu) {
		ReturnToMenu->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnReturnToMenuButtonClicked);
	}

	if (RestartLevel) {
		RestartLevel->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnRestartLevelButtonClicked);
	}

}

void UDieMenuOverlay::UpdateTextBlock(int32 SlayedEnemy, int32 LeftEnemy)
{
	FString CombinedString = FString::Printf(TEXT("You cut %d enemies, but %d enemies still remain in the world."), SlayedEnemy, LeftEnemy);
	FText NewText = FText::FromString(CombinedString);
	Text->SetText(NewText);
}

void UDieMenuOverlay::OnReturnToMenuButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
	UWorld* World = GetWorld(); // Get the UWorld pointer.
	if (World != nullptr) // Always check if the pointer is valid.
	{
		FString mapName = TEXT("/Game/Maps/LobbyLevel"); // Change this to your map's name.
		UGameplayStatics::OpenLevel(World, FName(*mapName)); // Open the specified level.
	}
}

void UDieMenuOverlay::OnRestartLevelButtonClicked()
{
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());
	FString mapName = GameInstance->GetSelectedMap();
	UGameplayStatics::OpenLevel(this, FName(*mapName));
}
