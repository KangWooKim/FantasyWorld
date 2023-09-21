// Fill out your copyright notice in the Description page of Project Settings.

// �ڵ� ������ ���� ��ó�� ���ù�. �� ������ �� ���� ���Եǰ� ��.
#pragma once

// CoreMinimal.h�� �����Ͽ� �𸮾� ������ �ֿ� ��� ������ ����� �� �ְ� ��.
#include "CoreMinimal.h"

// UObject/Interface.h�� �����Ͽ� �𸮾��� �������̽� ����� ����� �� �ְ� ��.
#include "UObject/Interface.h"

#include "PickupInterface.generated.h"

// UPickupInterface Ŭ������ �ڵ� ������ ���� ��ũ��.
// �� Ŭ������ ������ �ʿ䰡 ����.
UINTERFACE(MinimalAPI)
class UPickupInterface : public UInterface // UInterface�� ��ӹ���
{
	GENERATED_BODY() // �𸮾� ������ �ڵ� ���� ��ũ��
};

// IPickupInterface �������̽��� ������. ���� �������� �� �������̽��� ������ Ŭ������ �� �������̽��� ��ӹ޾ƾ� ��.
class FANTASYWORLD_API IPickupInterface
{
	GENERATED_BODY() // �𸮾� ������ �ڵ� ���� ��ũ��

		// �� Ŭ�������� �����ؾ� �� �������̽� �Լ����� ����.
public:

	// �������� ���ڷ� �޾� ó���ϴ� ���� �Լ�. Ŭ���� AItem�� ���ڷ� ����.
	virtual void SetOverlappingItem(class AItem* Item);

	// ��ȥ�� ���ڷ� �޾� ó���ϴ� ���� �Լ�. Ŭ���� ASoul�� ���ڷ� ����.
	virtual void AddSouls(class ASoul* Soul);

	// ���� ���ڷ� �޾� ó���ϴ� ���� �Լ�. Ŭ���� ATreasure�� ���ڷ� ����.
	virtual void AddGold(class ATreasure* Treasure);
};
