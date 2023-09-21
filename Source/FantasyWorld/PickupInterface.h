// Fill out your copyright notice in the Description page of Project Settings.

// 코드 생성을 위한 전처리 지시문. 이 파일은 한 번만 포함되게 함.
#pragma once

// CoreMinimal.h를 포함하여 언리얼 엔진의 주요 헤더 파일을 사용할 수 있게 함.
#include "CoreMinimal.h"

// UObject/Interface.h를 포함하여 언리얼의 인터페이스 기능을 사용할 수 있게 함.
#include "UObject/Interface.h"

#include "PickupInterface.generated.h"

// UPickupInterface 클래스의 코드 생성을 위한 매크로.
// 이 클래스는 수정할 필요가 없음.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface // UInterface를 상속받음
{
	GENERATED_BODY() // 언리얼 엔진의 코드 생성 매크로
};

// IPickupInterface 인터페이스를 정의함. 게임 로직에서 이 인터페이스를 구현할 클래스는 이 인터페이스를 상속받아야 함.
class FANTASYWORLD_API IPickupInterface
{
	GENERATED_BODY() // 언리얼 엔진의 코드 생성 매크로

		// 이 클래스에서 구현해야 할 인터페이스 함수들을 정의.
public:

	// 아이템을 인자로 받아 처리하는 가상 함수. 클래스 AItem을 인자로 받음.
	virtual void SetOverlappingItem(class AItem* Item);

	// 영혼을 인자로 받아 처리하는 가상 함수. 클래스 ASoul을 인자로 받음.
	virtual void AddSouls(class ASoul* Soul);

	// 금을 인자로 받아 처리하는 가상 함수. 클래스 ATreasure를 인자로 받음.
	virtual void AddGold(class ATreasure* Treasure);
};
