// Fill out your copyright notice in the Description page of Project Settings.


#include "Countess.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"



ACountess::ACountess() {
	
}

void ACountess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ACountess::Attack);
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &ACountess::LethalMode);
}

void ACountess::Attack()
{
	if (!CanAttack()) return;
	if (LethalState == ELethalState::ELS_On && SwingEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwingEffect, EquippedWeapon->GetActorLocation(), EquippedWeapon->GetActorRotation());
		Attributes->ReceiveDamage(SpendHealthAmount);
		if (CombatOverlay) {
				CombatOverlay->UpdateHealthBar(Attributes->GetHealthPercent());
		}
	}
	
	Super::Attack();
}

void ACountess::LethalMode()
{
	Super::LethalMode();

	if (LethalState == ELethalState::ELS_On) {
		if (EquippedWeapon) {
			EquippedWeapon->SetWeaponDamage(ELSDamage);
		}

		if (EquippedWeaponSecond) {
			EquippedWeaponSecond->SetWeaponDamage(ELSDamage);
		}
	}
	else {
		if (EquippedWeapon) {
			EquippedWeapon->SetWeaponDamage(NormalDamage);
		}

		if (EquippedWeaponSecond) {
			EquippedWeaponSecond->SetWeaponDamage(NormalDamage);
		}
	}
	
}

void ACountess::LethalModeFinish()
{
	Super::LethalModeFinish();

	if (EquippedWeapon) {
		EquippedWeapon->SetWeaponDamage(NormalDamage);
	}

	if (EquippedWeaponSecond) {
		EquippedWeaponSecond->SetWeaponDamage(NormalDamage);
	}
}

void ACountess::BeginPlay()
{
	Super::BeginPlay();
}

void ACountess::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (LethalState == ELethalState::ELS_On) {
		TimeSinceLastDecrease += DeltaTime;
		if (TimeSinceLastDecrease >= 1.0f) // 1초가 경과했는지 확인
		{
			Attributes->AddSouls(-1); // 필살기 시전 중에는 초당 soul을 1 소모합니다.
			TimeSinceLastDecrease -= 1.0f; // 시간 리셋
			if (CombatOverlay) {
				CombatOverlay->UpdateSoulText(Attributes->GetSouls());
			}
		}
	}
	if (LethalState == ELethalState::ELS_On && Attributes->GetSouls() == 0) {
		LethalModeFinish();
	}
}