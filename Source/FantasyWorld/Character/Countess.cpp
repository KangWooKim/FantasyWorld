// Fill out your copyright notice in the Description page of Project Settings.


#include "Countess.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"



// ACountess Ŭ������ �⺻ �������Դϴ�. ���� Ư���� �۾��� �������� �ʽ��ϴ�.
ACountess::ACountess() {

}

// �Է� ������Ʈ�� �����մϴ�. "Attack"�� "LethalMode" �׼��� ���� ACountess::Attack()�� ACountess::LethalMode() �޼��忡 �����մϴ�.
void ACountess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// �θ� Ŭ������ �Է� ������Ʈ ������ ȣ��
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// "Attack" �׼��� ACountess::Attack �޼��忡 ����
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ACountess::Attack);
	// "LethalMode" �׼��� ACountess::LethalMode �޼��忡 ����
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &ACountess::LethalMode);
}

// ���� �׼��� �����մϴ�.
void ACountess::Attack()
{
	// CanAttack() �޼���� ���� �������� Ȯ��
	if (!CanAttack()) return;

	// �ʻ� ���(ELethalState::ELS_On)�� ���
	if (LethalState == ELethalState::ELS_On && SwingEffect) {
		// ���� ����Ʈ ���
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwingEffect, EquippedWeapon->GetActorLocation(), EquippedWeapon->GetActorRotation());
		// �÷��̾��� �Ӽ�(Attributes)���� Ư�� ���ط�(SpendHealthAmount)�� ����
		Attributes->ReceiveDamage(SpendHealthAmount);
		// ���� ��������(CombatOverlay)���� ü�¹ٸ� ������Ʈ
		if (CombatOverlay) {
			CombatOverlay->UpdateHealthBar(Attributes->GetHealthPercent());
		}
	}

	// �θ� Ŭ������ Attack �޼��� ȣ��
	Super::Attack();
}

// �ʻ� ��带 �����մϴ�.
void ACountess::LethalMode()
{
	// �θ� Ŭ������ LethalMode �޼��� ȣ��
	Super::LethalMode();

	// �ʻ� ���(ELethalState::ELS_On)�� ���
	if (LethalState == ELethalState::ELS_On) {
		// �ֹ���� ���������� �������� ELSDamage�� ����
		if (EquippedWeapon) {
			EquippedWeapon->SetWeaponDamage(ELSDamage);
		}

		if (EquippedWeaponSecond) {
			EquippedWeaponSecond->SetWeaponDamage(ELSDamage);
		}
	}
	// �ʻ� ��尡 �ƴ� ���
	else {
		// �ֹ���� ���������� �������� NormalDamage�� ����
		if (EquippedWeapon) {
			EquippedWeapon->SetWeaponDamage(NormalDamage);
		}

		if (EquippedWeaponSecond) {
			EquippedWeaponSecond->SetWeaponDamage(NormalDamage);
		}
	}

}

// �ʻ� ��带 �����մϴ�.
void ACountess::LethalModeFinish()
{
	// �θ� Ŭ������ LethalModeFinish �޼��� ȣ��
	Super::LethalModeFinish();

	// �ֹ���� ���������� �������� NormalDamage�� �缳��
	if (EquippedWeapon) {
		EquippedWeapon->SetWeaponDamage(NormalDamage);
	}

	if (EquippedWeaponSecond) {
		EquippedWeaponSecond->SetWeaponDamage(NormalDamage);
	}
}

// BeginPlay �޼���, ������ ���۵ǰų� Actor�� ������ �� ȣ��˴ϴ�.
void ACountess::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay ȣ��
	Super::BeginPlay();
}

// Tick �޼���, �����Ӹ��� ȣ��˴ϴ�.
void ACountess::Tick(float DeltaTime)
{
	// �θ� Ŭ������ Tick �޼��� ȣ��
	Super::Tick(DeltaTime);

	// �ʻ� ���(ELethalState::ELS_On)�� ���
	if (LethalState == ELethalState::ELS_On) {
		// ��� �ð��� ������Ʈ
		TimeSinceLastDecrease += DeltaTime;
		if (TimeSinceLastDecrease >= 1.0f) // 1�ʰ� ����ߴ��� Ȯ��
		{
			// �ʴ� �ҿ� 1 ����
			Attributes->AddSouls(-1);
			// �ð� ����
			TimeSinceLastDecrease -= 1.0f;
			// �ҿ� �ؽ�Ʈ ������Ʈ
			if (CombatOverlay) {
				CombatOverlay->UpdateSoulText(Attributes->GetSouls());
			}
		}
	}
	// �ʻ� ����̸� �ҿ��� ���� ���, �ʻ� ��� ����
	if (LethalState == ELethalState::ELS_On && Attributes->GetSouls() == 0) {
		LethalModeFinish();
	}
}
