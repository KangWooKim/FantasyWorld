// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelHUD.h"
#include "TutorialLevelOverlay.h"

// ATutorialLevelHUD 클래스의 생성자 함수
// 현재는 아무 작업도 하지 않음
ATutorialLevelHUD::ATutorialLevelHUD() {

}

// BeginPlay는 Actor가 게임에 스폰될 때 한 번만 호출되는 함수
void ATutorialLevelHUD::BeginPlay()
{
	// 부모 클래스의 BeginPlay 호출
	Super::BeginPlay();

	// World 객체를 가져오거나 이미 있다면 그대로 사용
	// World는 현재 게임의 모든 엔터티를 포함하는 객체
	World = World == nullptr ? GetWorld() : World;

	// World 객체가 유효하다면 PlayerController를 가져옴
	// PlayerController는 사용자의 입력을 처리하는 객체
	if (World) {
		PlayerController = PlayerController == nullptr ? World->GetFirstPlayerController() : PlayerController;
	}

	// PlayerController와 TutorialLevelOverlayClass가 유효하다면
	// 마우스 커서를 보이게 하고 TutorialLevelOverlay 위젯을 생성하여 뷰포트에 추가
	if (PlayerController && TutorialLevelOverlayClass) {
		PlayerController->bShowMouseCursor = true; // 마우스 커서 보이게 설정
		// TutorialLevelOverlay 위젯 생성
		TutorialLevelOverlay = CreateWidget<UTutorialLevelOverlay>(PlayerController, TutorialLevelOverlayClass);
		TutorialLevelOverlay->AddToViewport(); // 생성된 위젯을 뷰포트에 추가
	}
}

