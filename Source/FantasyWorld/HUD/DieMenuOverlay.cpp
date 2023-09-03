// Fill out your copyright notice in the Description page of Project Settings.


#include "DieMenuOverlay.h"
#include "Components/Button.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Components/TextBlock.h"

// NativeConstruct�� UserWidget Ŭ�������� �������̵��� �Լ���, UI�� �����Ǿ��� �� ȣ��˴ϴ�.
void UDieMenuOverlay::NativeConstruct()
{
	Super::NativeConstruct();  // �θ� Ŭ������ NativeConstruct ȣ��

	// 'ReturnToMenu' ��ư�� nullptr�� �ƴ϶�� Ŭ�� �̺�Ʈ�� �Լ��� ���ε��մϴ�.
	if (ReturnToMenu) {
		ReturnToMenu->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnReturnToMenuButtonClicked);
	}

	// 'RestartLevel' ��ư�� nullptr�� �ƴ϶�� Ŭ�� �̺�Ʈ�� �Լ��� ���ε��մϴ�.
	if (RestartLevel) {
		RestartLevel->OnClicked.AddDynamic(this, &UDieMenuOverlay::OnRestartLevelButtonClicked);
	}
}

// ���ӿ��� ���� �� ��Ÿ�� �ؽ�Ʈ�� ������Ʈ�մϴ�.
// SlayedEnemy: �÷��̾ ���� ���� ��, LeftEnemy: ���� �����ִ� ���� ��
void UDieMenuOverlay::UpdateTextBlock(int32 SlayedEnemy, int32 LeftEnemy)
{
	FString CombinedString = FString::Printf(TEXT("You cut %d enemies, but %d enemies still remain in the world."), SlayedEnemy, LeftEnemy);
	FText NewText = FText::FromString(CombinedString);
	Text->SetText(NewText);  // �ؽ�Ʈ ��Ͽ� ���ο� �ؽ�Ʈ�� �����մϴ�.
}

// 'ReturnToMenu' ��ư�� Ŭ���Ǹ� ȣ��Ǵ� �Լ��Դϴ�.
void UDieMenuOverlay::OnReturnToMenuButtonClicked()
{
	// ��ư�� Ŭ���� �� ���带 ����մϴ�.
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UWorld* World = GetWorld();  // UWorld �����͸� �����ɴϴ�.
	// �����Ͱ� ��ȿ���� Ȯ���մϴ�.
	if (World != nullptr)
	{
		FString mapName = TEXT("/Game/Maps/LobbyLevel");  // �κ� ������ �̸��Դϴ�.
		UGameplayStatics::OpenLevel(World, FName(*mapName));  // ������ ������ ���� �Լ��� ȣ���մϴ�.
	}
}

// 'RestartLevel' ��ư�� Ŭ���Ǹ� ȣ��Ǵ� �Լ��Դϴ�.
void UDieMenuOverlay::OnRestartLevelButtonClicked()
{
	// ��ư�� Ŭ���� �� ���带 ����մϴ�.
	if (ButtonClickedSound) {
		UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}

	UMyGameInstance* GameInstance = Cast<UMyGameInstance>(GetGameInstance());  // ���� �ν��Ͻ��� �����ɴϴ�.
	FString mapName = GameInstance->GetSelectedMap();  // ���� �ν��Ͻ����� ���õ� ���� �̸��� �����ɴϴ�.
	UGameplayStatics::OpenLevel(this, FName(*mapName));  // ������ ������ ���� ������մϴ�.
}
