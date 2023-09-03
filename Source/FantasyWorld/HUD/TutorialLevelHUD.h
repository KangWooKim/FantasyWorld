// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TutorialLevelHUD.generated.h"

class UTutorialLevelOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATutorialLevelHUD : public AHUD
{
	GENERATED_BODY()

		// public 섹션의 멤버 함수와 변수 정의
public:

	// ATutorialLevelHUD 클래스의 기본 생성자
	ATutorialLevelHUD();

	// BeginPlay 함수를 오버라이드. 게임 플레이가 시작될 때 한 번 호출됨
	virtual void BeginPlay() override;

	// TutorialLevelOverlay 멤버 변수를 반환하는 간단한 인라인 함수
	// FORCEINLINE은 컴파일러에게 가능하면 인라인으로 만들라고 지시
	FORCEINLINE UTutorialLevelOverlay* GetTutorialLevelOverlay() { return TutorialLevelOverlay; }

	// protected 섹션. 상속받은 클래스에서 접근 가능
protected:

	// private 섹션. 해당 클래스 내부에서만 접근 가능
private:

	// 현재 게임의 World 객체를 저장하는 포인터
	UWorld* World;

	// 현재 게임의 PlayerController 객체를 저장하는 포인터
	APlayerController* PlayerController;

	// TutorialLevelOverlayClass의 타입 정보를 저장. 
	// EditDefaultsOnly는 에디터에서만 디폴트 값을 수정할 수 있게 한다.
	UPROPERTY(EditDefaultsOnly, Category = "Media")
		TSubclassOf<UTutorialLevelOverlay> TutorialLevelOverlayClass;

	// TutorialLevelOverlay 객체를 저장하는 포인터.
	UPROPERTY()
		UTutorialLevelOverlay* TutorialLevelOverlay;

};
