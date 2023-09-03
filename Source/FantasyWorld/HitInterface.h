// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

// CoreMinimal.h�� �����Ͽ� �𸮾� ������ �ֿ� ��� ������ ����� �� �ְ� ��
#include "CoreMinimal.h"

// UObject/Interface.h�� �����Ͽ� �𸮾��� �������̽� ����� ����� �� �ְ� ��
#include "UObject/Interface.h"

// UHitInterface Ŭ������ �ڵ� ������ ���� ��ũ��
// �� Ŭ������ ������ �ʿ䰡 ����.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface // UInterface�� ��ӹ���
{
	GENERATED_BODY() // �𸮾� ������ �ڵ� ���� ��ũ��
};

// IHitInterface �������̽��� ������. ���� �������� �� �������̽��� ������ Ŭ������ �� �������̽��� ��ӹ޾ƾ� ��.
class FANTASYWORLD_API IHitInterface
{
	GENERATED_BODY() // �𸮾� ������ �ڵ� ���� ��ũ��

		// �� Ŭ�������� �����ؾ� �ϴ� �������̽� �Լ��� �߰�. �� Ŭ������ ��ӹ޾� �� �������̽��� �����ؾ� ��.
public:

	// GetHit �Լ��� ����. �� �Լ��� �������Ʈ���� ����Ƽ�� �̺�Ʈ�ε� ����� �� ����.
	// InpactPoint: �浹�� �߻��� ��ġ
	// Hitter: �浹�� �߻���Ų ����
	UFUNCTION(BlueprintNativeEvent)
		void GetHit(const FVector& InpactPoint, AActor* Hitter);
};
