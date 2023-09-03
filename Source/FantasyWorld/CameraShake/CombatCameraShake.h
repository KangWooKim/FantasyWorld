// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// 레거시 카메라 쉐이크 클래스를 위한 헤더를 포함시킵니다.
#include "LegacyCameraShake.h"

// 클래스가 어떤 형태로 생성될지 메타데이터를 정의합니다.
#include "CombatCameraShake.generated.h"

/**
 * 
 */
 // UCombatCameraShake 클래스를 선언합니다. 이 클래스는 ULegacyCameraShake 클래스를 상속받습니다.
UCLASS()
class FANTASYWORLD_API UCombatCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()

public : 
	// 기본 생성자를 선언합니다.
	UCombatCameraShake();

protected:

private:

};
