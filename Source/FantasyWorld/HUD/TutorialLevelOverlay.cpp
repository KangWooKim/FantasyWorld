// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelOverlay.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"


// NativeConstruct �Լ��� UUserWidget�� �����ǰ� �ε�Ǹ� �ڵ����� ȣ���.
// �� �Լ� �������� �� Ŭ������ ����ϴ� �پ��� ������ �̺�Ʈ ���ε��� ������.
void UTutorialLevelOverlay::NativeConstruct()
{
    // �θ� Ŭ������ NativeConstruct�� ȣ���Ͽ� �⺻ ������ ������.
    Super::NativeConstruct();

    // ���� ������ �ε����� 0���� ����. ���� ���� ������ ����� �����̶�� �� �ε����� ����� ����.
    CurrentVideoIndex = 0;

    // OkButton�� �����Ѵٸ�, OnOkButtonClicked �Լ��� �ش� ��ư�� Ŭ�� �̺�Ʈ�� ���ε���.
    if (OkButton) {
        OkButton->OnClicked.AddDynamic(this, &UTutorialLevelOverlay::OnOkButtonClicked);
    }

    // QuitButton�� �����Ѵٸ�, OnQuitButtonClicked �Լ��� �ش� ��ư�� Ŭ�� �̺�Ʈ�� ���ε���.
    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &UTutorialLevelOverlay::OnQuitButtonClicked);
    }

    // MediaSources �迭�� �̹� ä���� �ִٰ� ������.
    // MediaSources �迭�� ��� �ִٸ�, UMediaSource ��ü�� �߰��ؾ� ��.
    if (MediaSources.Num() > 0) {
        // ù ��° �̵�� �ҽ��� ���� �����.
        MediaPlayer->OpenSource(MediaSources[0]);
        MediaPlayer->Play();
    }

    // MediaTexture�� �̵�� �÷��̾ ������.
    MediaTexture->SetMediaPlayer(MediaPlayer);

    // MediaTexture�� Brush�� ���ҽ� ��ü�� ������.
    Brush.SetResourceObject(MediaTexture);
    // Image ������ Brush�� ������.
    Image->SetBrush(Brush);
}

// OkButton�� Ŭ���� �� ȣ��Ǵ� �Լ�
void UTutorialLevelOverlay::OnOkButtonClicked()
{
    // ButtonClickedSound�� �����Ѵٸ�, ���带 �����.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // ���콺 Ŀ���� ����.
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

    // ù ��° �÷��̾� ��Ʈ�ѷ��� �� AFantasyPlayerController�� ����ȯ��.
    AFantasyPlayerController* Controller = Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController());

    // �÷��̾ �ٽ� Possess ���·� ����.
    Controller->RePossess();

    // �� ������ ����.
    this->SetVisibility(ESlateVisibility::Hidden);
}


// QuitButton�� Ŭ���� �� ȣ��Ǵ� �Լ�
void UTutorialLevelOverlay::OnQuitButtonClicked()
{
    // ButtonClickedSound�� �����Ѵٸ�, ���带 �����.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // ���� UWorld �����͸� ����.
    UWorld* World = GetWorld();

    // �����Ͱ� ��ȿ���� �׻� Ȯ����.
    if (World != nullptr)
    {
        // ������ �ε��� ���� �̸��� ������.
        FString mapName = TEXT("/Game/Maps/LobbyLevel");

        // ������ ����(��)�� ����.
        UGameplayStatics::OpenLevel(World, FName(*mapName));
    }
}

// ���� ������ ����ϴ� �Լ�
void UTutorialLevelOverlay::PlayNextVideo()
{
    // ������ Ȱ��ȭ��.
    this->SetVisibility(ESlateVisibility::Visible);

    // ���� ���� �ε����� ������Ŵ.
    ++CurrentVideoIndex;

    // ���ο� �ε����� ��ȿ���� Ȯ���� �� ������ �����.
    if (MediaSources.IsValidIndex(CurrentVideoIndex)) {
        MediaPlayer->OpenSource(MediaSources[CurrentVideoIndex]);
        MediaPlayer->Play();
    }

    // MediaTexture�� ������.
    if (MediaTexture) {
        MediaTexture->SetMediaPlayer(MediaPlayer);
    }

    // Brush�� Image ������ ������.
    Brush.SetResourceObject(MediaTexture);
    Image->SetBrush(Brush);

    // ������ ������� OkButton�� ����.
    if (CurrentVideoIndex == MediaSources.Num() - 1) {
        OkButton->SetVisibility(ESlateVisibility::Hidden);
    }
}

// �� �Լ��� ������ ���ü��� Ȱ��ȭ�ϰ� ���� ������ �����.
void UTutorialLevelOverlay::VisibilityEnabled()
{
    // ������ Ȱ��ȭ��.
    this->SetVisibility(ESlateVisibility::Visible);

    // ���콺 Ŀ���� ���̰� ��.
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

    // ���� ������ �����.
    PlayNextVideo();
}
