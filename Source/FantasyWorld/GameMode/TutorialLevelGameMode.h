// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TutorialLevelGameMode.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATutorialLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

    ATutorialLevelGameMode();  // 기본 생성자입니다. 게임 모드의 초기 설정을 여기서 할 수 있습니다.
    virtual void BeginPlay() override;  // BeginPlay 함수를 오버라이딩합니다. 게임이 시작될 때 호출되는 함수입니다.

protected:
    // 보호된 멤버 변수나 함수는 여기에 위치합니다.

private:

    UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
        USoundBase* BackGroundMusic;  // 배경 음악을 설정할 수 있는 속성입니다. 에디터에서 이 값을 수정할 수 있습니다.

};
