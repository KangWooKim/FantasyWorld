// Fill out your copyright notice in the Description page of Project Settings.


// PickupInterface.h ��� ������ �����Ͽ� IPickupInterface �������̽��� ������ ����� �� �ְ� ��.
#include "PickupInterface.h"

// �Ʒ� �ڵ�� IPickupInterface�� �޼��忡 ���� �⺻ ������ �����մϴ�. 
// �̴� ���� ���� �Լ��� �ƴϹǷ� �⺻ ������ ������ �� �ֽ��ϴ�.

// SetOverlappingItem �޼����� �⺻ ����. �ƹ��͵� ���� ����.
// AItem* Item: ó���ؾ� �� ������ ��ü�� ����Ű�� ������.
void IPickupInterface::SetOverlappingItem(AItem* Item)
{
    // �⺻ ������ �ƹ� �͵� ���� �ʽ��ϴ�.
    // ���� Ŭ�������� �� �޼��带 �������̵��Ͽ� ���ϴ� ������ �߰��� �� �ֽ��ϴ�.
}

// AddSouls �޼����� �⺻ ����. �ƹ��͵� ���� ����.
// ASoul* Soul: ó���ؾ� �� ��ȥ ��ü�� ����Ű�� ������.
void IPickupInterface::AddSouls(ASoul* Soul)
{
    // �⺻ ������ �ƹ� �͵� ���� �ʽ��ϴ�.
    // ���� Ŭ�������� �� �޼��带 �������̵��Ͽ� ���ϴ� ������ �߰��� �� �ֽ��ϴ�.
}

// AddGold �޼����� �⺻ ����. �ƹ��͵� ���� ����.
// ATreasure* Treasure: ó���ؾ� �� ����(��) ��ü�� ����Ű�� ������.
void IPickupInterface::AddGold(ATreasure* Treasure)
{
    // �⺻ ������ �ƹ� �͵� ���� �ʽ��ϴ�.
    // ���� Ŭ�������� �� �޼��带 �������̵��Ͽ� ���ϴ� ������ �߰��� �� �ֽ��ϴ�.
}
