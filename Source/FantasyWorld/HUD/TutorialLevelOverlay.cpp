// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelOverlay.h"
#include "Engine/Texture2D.h"
#include "Components/Button.h"
#include "FileMediaSource.h"
#include "Styling/SlateBrush.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"


// NativeConstruct 함수는 UUserWidget이 생성되고 로드되면 자동으로 호출됨.
// 이 함수 내에서는 이 클래스가 사용하는 다양한 설정과 이벤트 바인딩을 수행함.
void UTutorialLevelOverlay::NativeConstruct()
{
    // 부모 클래스의 NativeConstruct를 호출하여 기본 설정을 적용함.
    Super::NativeConstruct();

    // 현재 비디오의 인덱스를 0으로 설정. 여러 개의 비디오를 재생할 예정이라면 이 인덱스가 변경될 것임.
    CurrentVideoIndex = 0;

    // OkButton이 존재한다면, OnOkButtonClicked 함수를 해당 버튼의 클릭 이벤트에 바인딩함.
    if (OkButton) {
        OkButton->OnClicked.AddDynamic(this, &UTutorialLevelOverlay::OnOkButtonClicked);
    }

    // QuitButton이 존재한다면, OnQuitButtonClicked 함수를 해당 버튼의 클릭 이벤트에 바인딩함.
    if (QuitButton) {
        QuitButton->OnClicked.AddDynamic(this, &UTutorialLevelOverlay::OnQuitButtonClicked);
    }

    // MediaSources 배열이 이미 채워져 있다고 가정함.
    // MediaSources 배열이 비어 있다면, UMediaSource 객체를 추가해야 함.
    if (MediaSources.Num() > 0) {
        // 첫 번째 미디어 소스를 열고 재생함.
        MediaPlayer->OpenSource(MediaSources[0]);
        MediaPlayer->Play();
    }

    // MediaTexture의 미디어 플레이어를 설정함.
    MediaTexture->SetMediaPlayer(MediaPlayer);

    // MediaTexture를 Brush의 리소스 객체로 설정함.
    Brush.SetResourceObject(MediaTexture);
    // Image 위젯의 Brush를 설정함.
    Image->SetBrush(Brush);
}

// OkButton이 클릭될 때 호출되는 함수
void UTutorialLevelOverlay::OnOkButtonClicked()
{
    // ButtonClickedSound가 존재한다면, 사운드를 재생함.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 마우스 커서를 숨김.
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;

    // 첫 번째 플레이어 컨트롤러를 얻어서 AFantasyPlayerController로 형변환함.
    AFantasyPlayerController* Controller = Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController());

    // 플레이어를 다시 Possess 상태로 만듦.
    Controller->RePossess();

    // 이 위젯을 숨김.
    this->SetVisibility(ESlateVisibility::Hidden);
}


// QuitButton이 클릭될 때 호출되는 함수
void UTutorialLevelOverlay::OnQuitButtonClicked()
{
    // ButtonClickedSound가 존재한다면, 사운드를 재생함.
    if (ButtonClickedSound) {
        UGameplayStatics::PlaySound2D(this, ButtonClickedSound, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }

    // 현재 UWorld 포인터를 얻어옴.
    UWorld* World = GetWorld();

    // 포인터가 유효한지 항상 확인함.
    if (World != nullptr)
    {
        // 다음에 로딩할 맵의 이름을 지정함.
        FString mapName = TEXT("/Game/Maps/LobbyLevel");

        // 지정된 레벨(맵)을 열음.
        UGameplayStatics::OpenLevel(World, FName(*mapName));
    }
}

// 다음 비디오를 재생하는 함수
void UTutorialLevelOverlay::PlayNextVideo()
{
    // 위젯을 활성화함.
    this->SetVisibility(ESlateVisibility::Visible);

    // 현재 비디오 인덱스를 증가시킴.
    ++CurrentVideoIndex;

    // 새로운 인덱스가 유효한지 확인한 후 비디오를 재생함.
    if (MediaSources.IsValidIndex(CurrentVideoIndex)) {
        MediaPlayer->OpenSource(MediaSources[CurrentVideoIndex]);
        MediaPlayer->Play();
    }

    // MediaTexture를 설정함.
    if (MediaTexture) {
        MediaTexture->SetMediaPlayer(MediaPlayer);
    }

    // Brush와 Image 위젯을 설정함.
    Brush.SetResourceObject(MediaTexture);
    Image->SetBrush(Brush);

    // 마지막 비디오라면 OkButton을 숨김.
    if (CurrentVideoIndex == MediaSources.Num() - 1) {
        OkButton->SetVisibility(ESlateVisibility::Hidden);
    }
}

// 이 함수는 위젯의 가시성을 활성화하고 다음 비디오를 재생함.
void UTutorialLevelOverlay::VisibilityEnabled()
{
    // 위젯을 활성화함.
    this->SetVisibility(ESlateVisibility::Visible);

    // 마우스 커서를 보이게 함.
    GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;

    // 다음 비디오를 재생함.
    PlayNextVideo();
}
