// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// ���Ž� ī�޶� ����ũ Ŭ������ ���� ����� ���Խ�ŵ�ϴ�.
#include "LegacyCameraShake.h"

// Ŭ������ � ���·� �������� ��Ÿ�����͸� �����մϴ�.
#include "CombatCameraShake.generated.h"

/**
 * 
 */
 // UCombatCameraShake Ŭ������ �����մϴ�. �� Ŭ������ ULegacyCameraShake Ŭ������ ��ӹ޽��ϴ�.
UCLASS()
class FANTASYWORLD_API UCombatCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()

public : 
	// �⺻ �����ڸ� �����մϴ�.
	UCombatCameraShake();

protected:

private:

};
