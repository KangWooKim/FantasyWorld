// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "HealthBarComponent.generated.h"

/**
 *
 */
UCLASS()
class FANTASYWORLD_API UHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UHealthBarComponent(); // ������. �� ������Ʈ�� ������ �� ȣ��˴ϴ�.

	// �������̵��� TickComponent �޼���. ���ӿ��� �� �����Ӹ��� ȣ��˴ϴ�.
	// DeltaTime: ���� ƽ�� ���� ƽ ������ �ð�(��)
	// TickType: �� ƽ�� Ÿ��(��: ���� ����, ���� ���� ��)
	// ThisTickFunction: �� ƽ�� ó���ϴ� �Լ��� ���� ����
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ü�¹��� �ۼ�Ƽ���� �����ϴ� �Լ��Դϴ�.
	// Percent: ���� ������ ü�¹��� �ۼ�Ʈ (0.0 ~ 1.0)
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
		class UHealthBar* HealthBarWidget; // �� ������Ʈ�� ������ UHealthBar ����

	UPROPERTY()
		class APlayerController* PlayerController; // �÷��̾� ��Ʈ�ѷ��� ����. ī�޶��� ��ġ�� ȸ���� ��� ���� ���˴ϴ�.

};
