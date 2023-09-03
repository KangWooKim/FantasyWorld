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

	UHealthBarComponent(); // 생성자. 이 컴포넌트가 생성될 때 호출됩니다.

	// 오버라이딩한 TickComponent 메서드. 게임에서 매 프레임마다 호출됩니다.
	// DeltaTime: 이전 틱과 현재 틱 사이의 시간(초)
	// TickType: 이 틱의 타입(예: 게임 루프, 랜더 루프 등)
	// ThisTickFunction: 이 틱을 처리하는 함수에 대한 정보
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 체력바의 퍼센티지를 설정하는 함수입니다.
	// Percent: 새로 설정할 체력바의 퍼센트 (0.0 ~ 1.0)
	void SetHealthPercent(float Percent);

private:
	UPROPERTY()
		class UHealthBar* HealthBarWidget; // 이 컴포넌트가 제어할 UHealthBar 위젯

	UPROPERTY()
		class APlayerController* PlayerController; // 플레이어 컨트롤러의 참조. 카메라의 위치와 회전을 얻기 위해 사용됩니다.

};
