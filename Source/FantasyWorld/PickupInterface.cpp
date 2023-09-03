// Fill out your copyright notice in the Description page of Project Settings.


// PickupInterface.h 헤더 파일을 포함하여 IPickupInterface 인터페이스의 선언을 사용할 수 있게 함.
#include "PickupInterface.h"

// 아래 코드는 IPickupInterface의 메서드에 대한 기본 구현을 제공합니다. 
// 이는 순수 가상 함수가 아니므로 기본 구현을 제공할 수 있습니다.

// SetOverlappingItem 메서드의 기본 구현. 아무것도 하지 않음.
// AItem* Item: 처리해야 할 아이템 객체를 가리키는 포인터.
void IPickupInterface::SetOverlappingItem(AItem* Item)
{
    // 기본 구현은 아무 것도 하지 않습니다.
    // 구현 클래스에서 이 메서드를 오버라이딩하여 원하는 로직을 추가할 수 있습니다.
}

// AddSouls 메서드의 기본 구현. 아무것도 하지 않음.
// ASoul* Soul: 처리해야 할 영혼 객체를 가리키는 포인터.
void IPickupInterface::AddSouls(ASoul* Soul)
{
    // 기본 구현은 아무 것도 하지 않습니다.
    // 구현 클래스에서 이 메서드를 오버라이딩하여 원하는 로직을 추가할 수 있습니다.
}

// AddGold 메서드의 기본 구현. 아무것도 하지 않음.
// ATreasure* Treasure: 처리해야 할 보물(금) 객체를 가리키는 포인터.
void IPickupInterface::AddGold(ATreasure* Treasure)
{
    // 기본 구현은 아무 것도 하지 않습니다.
    // 구현 클래스에서 이 메서드를 오버라이딩하여 원하는 로직을 추가할 수 있습니다.
}
