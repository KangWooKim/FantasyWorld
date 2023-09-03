// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// CoreMinimal.h를 포함하여 언리얼 엔진의 주요 헤더 파일을 사용할 수 있게 함
#include "CoreMinimal.h"

// UObject/Interface.h를 포함하여 언리얼의 인터페이스 기능을 사용할 수 있게 함
#include "UObject/Interface.h"

// UHitInterface 클래스의 코드 생성을 위한 매크로
// 이 클래스는 수정할 필요가 없음.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface // UInterface를 상속받음
{
	GENERATED_BODY() // 언리얼 엔진의 코드 생성 매크로
};

// IHitInterface 인터페이스를 정의함. 게임 로직에서 이 인터페이스를 구현할 클래스는 이 인터페이스를 상속받아야 함.
class FANTASYWORLD_API IHitInterface
{
	GENERATED_BODY() // 언리얼 엔진의 코드 생성 매크로

		// 이 클래스에서 구현해야 하는 인터페이스 함수를 추가. 이 클래스를 상속받아 이 인터페이스를 구현해야 함.
public:

	// GetHit 함수를 선언. 이 함수는 블루프린트에서 네이티브 이벤트로도 사용할 수 있음.
	// InpactPoint: 충돌이 발생한 위치
	// Hitter: 충돌을 발생시킨 액터
	UFUNCTION(BlueprintNativeEvent)
		void GetHit(const FVector& InpactPoint, AActor* Hitter);
};
