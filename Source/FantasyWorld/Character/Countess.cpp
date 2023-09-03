// Fill out your copyright notice in the Description page of Project Settings.


#include "Countess.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"



// ACountess 클래스의 기본 생성자입니다. 현재 특별한 작업을 수행하지 않습니다.
ACountess::ACountess() {

}

// 입력 컴포넌트를 설정합니다. "Attack"과 "LethalMode" 액션을 각각 ACountess::Attack()과 ACountess::LethalMode() 메서드에 연결합니다.
void ACountess::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// 부모 클래스의 입력 컴포넌트 설정을 호출
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// "Attack" 액션을 ACountess::Attack 메서드에 연결
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ACountess::Attack);
	// "LethalMode" 액션을 ACountess::LethalMode 메서드에 연결
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &ACountess::LethalMode);
}

// 공격 액션을 수행합니다.
void ACountess::Attack()
{
	// CanAttack() 메서드로 공격 가능한지 확인
	if (!CanAttack()) return;

	// 필살 모드(ELethalState::ELS_On)인 경우
	if (LethalState == ELethalState::ELS_On && SwingEffect) {
		// 스윙 이펙트 출력
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SwingEffect, EquippedWeapon->GetActorLocation(), EquippedWeapon->GetActorRotation());
		// 플레이어의 속성(Attributes)에서 특정 피해량(SpendHealthAmount)을 적용
		Attributes->ReceiveDamage(SpendHealthAmount);
		// 전투 오버레이(CombatOverlay)에서 체력바를 업데이트
		if (CombatOverlay) {
			CombatOverlay->UpdateHealthBar(Attributes->GetHealthPercent());
		}
	}

	// 부모 클래스의 Attack 메서드 호출
	Super::Attack();
}

// 필살 모드를 수행합니다.
void ACountess::LethalMode()
{
	// 부모 클래스의 LethalMode 메서드 호출
	Super::LethalMode();

	// 필살 모드(ELethalState::ELS_On)인 경우
	if (LethalState == ELethalState::ELS_On) {
		// 주무기와 보조무기의 데미지를 ELSDamage로 설정
		if (EquippedWeapon) {
			EquippedWeapon->SetWeaponDamage(ELSDamage);
		}

		if (EquippedWeaponSecond) {
			EquippedWeaponSecond->SetWeaponDamage(ELSDamage);
		}
	}
	// 필살 모드가 아닌 경우
	else {
		// 주무기와 보조무기의 데미지를 NormalDamage로 설정
		if (EquippedWeapon) {
			EquippedWeapon->SetWeaponDamage(NormalDamage);
		}

		if (EquippedWeaponSecond) {
			EquippedWeaponSecond->SetWeaponDamage(NormalDamage);
		}
	}

}

// 필살 모드를 종료합니다.
void ACountess::LethalModeFinish()
{
	// 부모 클래스의 LethalModeFinish 메서드 호출
	Super::LethalModeFinish();

	// 주무기와 보조무기의 데미지를 NormalDamage로 재설정
	if (EquippedWeapon) {
		EquippedWeapon->SetWeaponDamage(NormalDamage);
	}

	if (EquippedWeaponSecond) {
		EquippedWeaponSecond->SetWeaponDamage(NormalDamage);
	}
}

// BeginPlay 메서드, 게임이 시작되거나 Actor가 스폰될 때 호출됩니다.
void ACountess::BeginPlay()
{
	// 부모 클래스의 BeginPlay 호출
	Super::BeginPlay();
}

// Tick 메서드, 프레임마다 호출됩니다.
void ACountess::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick 메서드 호출
	Super::Tick(DeltaTime);

	// 필살 모드(ELethalState::ELS_On)인 경우
	if (LethalState == ELethalState::ELS_On) {
		// 경과 시간을 업데이트
		TimeSinceLastDecrease += DeltaTime;
		if (TimeSinceLastDecrease >= 1.0f) // 1초가 경과했는지 확인
		{
			// 초당 소울 1 감소
			Attributes->AddSouls(-1);
			// 시간 리셋
			TimeSinceLastDecrease -= 1.0f;
			// 소울 텍스트 업데이트
			if (CombatOverlay) {
				CombatOverlay->UpdateSoulText(Attributes->GetSouls());
			}
		}
	}
	// 필살 모드이며 소울이 없는 경우, 필살 모드 종료
	if (LethalState == ELethalState::ELS_On && Attributes->GetSouls() == 0) {
		LethalModeFinish();
	}
}
