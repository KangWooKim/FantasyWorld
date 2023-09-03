// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// 기본 생성자: HealthBarComponent 클래스의 인스턴스를 초기화
UHealthBarComponent::UHealthBarComponent()
{
	// 이 컴포넌트가 Tick 함수를 호출할 수 있도록 틱을 활성화합니다.
	PrimaryComponentTick.bCanEverTick = true;
}

// 틱 함수: 게임의 각 프레임에서 호출됩니다.
void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 부모 클래스의 TickComponent를 호출하여 기본 기능을 유지합니다.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 플레이어 컨트롤러를 얻어옵니다. 이미 할당되어 있지 않다면 GetWorld 함수를 통해 얻습니다.
	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;

	// 플레이어 컨트롤러가 유효하다면
	if (PlayerController)
	{
		// 플레이어의 카메라 위치와 회전을 얻습니다.
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 체력바가 플레이어를 바라보도록 회전 벡터를 계산합니다.
		FVector Direction = CameraLocation - GetOwner()->GetActorLocation();
		Direction.Normalize();

		// 새로운 회전 값을 계산합니다.
		FRotator NewRotation = Direction.Rotation();
		NewRotation.Pitch = 0; // 체력바가 수평이 되도록 Pitch 값을 0으로 설정합니다.

		// 새로운 회전 값을 적용합니다.
		SetWorldRotation(NewRotation);
	}
}

// 체력바의 퍼센트를 설정하는 함수
void UHealthBarComponent::SetHealthPercent(float Percent) // Percent: 설정할 체력바의 퍼센트 값
{
	// HealthBarWidget이 nullptr인 경우에만 GetUserWidgetObject를 사용하여 할당합니다.
	if (HealthBarWidget == nullptr) {
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	// HealthBarWidget과 그 하위의 HealthBar가 유효하다면
	if (HealthBarWidget && HealthBarWidget->HealthBar) {
		// HealthBar의 퍼센트를 설정합니다.
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
