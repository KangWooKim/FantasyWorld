// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UHealthBarComponent::UHealthBarComponent()
{
	// 틱 활성화
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 플레이어 컨트롤러와 카메라 얻기
	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// 체력바가 카메라를 향하게 하려면 회전 벡터의 방향을 변경해야 함
		FVector Direction = CameraLocation - GetOwner()->GetActorLocation();
		Direction.Normalize();

		FRotator NewRotation = Direction.Rotation();
		NewRotation.Pitch = 0; // 위젯이 수평이 되도록 피치를 0으로 설정
		SetWorldRotation(NewRotation);
	}
}

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthBarWidget == nullptr) {
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	if (HealthBarWidget && HealthBarWidget->HealthBar) {
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
