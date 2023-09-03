// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

// 기본 생성자, 튜토리얼 게임 모드 초기화
ATutorialLevelGameMode::ATutorialLevelGameMode()
{
}

// BeginPlay 메서드, 게임 시작시 실행되는 함수
void ATutorialLevelGameMode::BeginPlay()
{
    // 부모 클래스의 BeginPlay 호출
    Super::BeginPlay();

    // 배경 음악이 설정되어 있다면,
    if (BackGroundMusic) {
        // 배경 음악을 재생한다. 볼륨은 1.0, 피치는 1.0, 시작 시간은 0.0
        UGameplayStatics::PlaySound2D(this, BackGroundMusic, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }
}
