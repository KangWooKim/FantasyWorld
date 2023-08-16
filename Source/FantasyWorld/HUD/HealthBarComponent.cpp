// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

UHealthBarComponent::UHealthBarComponent()
{
	// ƽ Ȱ��ȭ
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �÷��̾� ��Ʈ�ѷ��� ī�޶� ���
	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;
	if (PlayerController)
	{
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// ü�¹ٰ� ī�޶� ���ϰ� �Ϸ��� ȸ�� ������ ������ �����ؾ� ��
		FVector Direction = CameraLocation - GetOwner()->GetActorLocation();
		Direction.Normalize();

		FRotator NewRotation = Direction.Rotation();
		NewRotation.Pitch = 0; // ������ ������ �ǵ��� ��ġ�� 0���� ����
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
