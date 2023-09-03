// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

// �⺻ ������: HealthBarComponent Ŭ������ �ν��Ͻ��� �ʱ�ȭ
UHealthBarComponent::UHealthBarComponent()
{
	// �� ������Ʈ�� Tick �Լ��� ȣ���� �� �ֵ��� ƽ�� Ȱ��ȭ�մϴ�.
	PrimaryComponentTick.bCanEverTick = true;
}

// ƽ �Լ�: ������ �� �����ӿ��� ȣ��˴ϴ�.
void UHealthBarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// �θ� Ŭ������ TickComponent�� ȣ���Ͽ� �⺻ ����� �����մϴ�.
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �÷��̾� ��Ʈ�ѷ��� ���ɴϴ�. �̹� �Ҵ�Ǿ� ���� �ʴٸ� GetWorld �Լ��� ���� ����ϴ�.
	PlayerController = PlayerController == nullptr ? GetWorld()->GetFirstPlayerController() : PlayerController;

	// �÷��̾� ��Ʈ�ѷ��� ��ȿ�ϴٸ�
	if (PlayerController)
	{
		// �÷��̾��� ī�޶� ��ġ�� ȸ���� ����ϴ�.
		FVector CameraLocation;
		FRotator CameraRotation;
		PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);

		// ü�¹ٰ� �÷��̾ �ٶ󺸵��� ȸ�� ���͸� ����մϴ�.
		FVector Direction = CameraLocation - GetOwner()->GetActorLocation();
		Direction.Normalize();

		// ���ο� ȸ�� ���� ����մϴ�.
		FRotator NewRotation = Direction.Rotation();
		NewRotation.Pitch = 0; // ü�¹ٰ� ������ �ǵ��� Pitch ���� 0���� �����մϴ�.

		// ���ο� ȸ�� ���� �����մϴ�.
		SetWorldRotation(NewRotation);
	}
}

// ü�¹��� �ۼ�Ʈ�� �����ϴ� �Լ�
void UHealthBarComponent::SetHealthPercent(float Percent) // Percent: ������ ü�¹��� �ۼ�Ʈ ��
{
	// HealthBarWidget�� nullptr�� ��쿡�� GetUserWidgetObject�� ����Ͽ� �Ҵ��մϴ�.
	if (HealthBarWidget == nullptr) {
		HealthBarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}

	// HealthBarWidget�� �� ������ HealthBar�� ��ȿ�ϴٸ�
	if (HealthBarWidget && HealthBarWidget->HealthBar) {
		// HealthBar�� �ۼ�Ʈ�� �����մϴ�.
		HealthBarWidget->HealthBar->SetPercent(Percent);
	}
}
