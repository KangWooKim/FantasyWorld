// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy.h"

// APhase 클래스의 기본 생성자입니다.
APhase::APhase()
{
}

// 입력 컴포넌트를 설정하는 함수입니다. "Attack"과 "LethalMode" 액션을 바인딩합니다.
void APhase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);  // 부모 클래스의 함수를 호출합니다.

	// "Attack"이라는 이름의 액션을 누르면 APhase::Attack 함수가 호출됩니다.
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &APhase::Attack);

	// "LethalMode"이라는 이름의 액션을 누르면 APhase::LethalMode 함수가 호출됩니다.
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &APhase::LethalMode);
}

// 캐릭터가 데미지를 받을 때 호출되는 함수입니다.
float APhase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 필살기 모드가 아니면 부모 클래스의 TakeDamage 함수를 호출합니다.
	if (LethalState == ELethalState::ELS_Off) {
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	// 필살기 모드일 경우, ShieldEffect를 생성하고 스태미나를 사용합니다.
	else if (LethalState == ELethalState::ELS_On) {
		// 쉴드 이펙트 생성
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldEffect, this->GetActorLocation(), this->GetActorRotation());

		// 데미지를 입힌 액터에게 역으로 데미지를 입힙니다.
		UGameplayStatics::ApplyDamage(DamageCauser->GetOwner(), DamageAmount, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());

		// 데미지를 입힌 액터가 IHitInterface를 구현하고 있다면, GetHit 함수를 호출합니다.
		IHitInterface* HitInterface = Cast<IHitInterface>(DamageCauser->GetOwner());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(DamageCauser->GetOwner(), DamageCauser->GetOwner()->GetActorLocation(), this);
		}

		// 스태미나를 소모합니다.
		Attributes->UseStamina(DamageAmount);

		// 스태미나 바 UI를 업데이트합니다.
		CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
	}
	return DamageAmount;
}

// 게임이 시작될 때 한 번만 호출됩니다.
void APhase::BeginPlay()
{
	Super::BeginPlay();  // 부모 클래스의 BeginPlay를 호출합니다.
}

// 프레임마다 호출되는 함수입니다.
void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  // 부모 클래스의 Tick 함수를 호출합니다.
}

// 캐릭터가 데미지를 받을 때 호출되는 함수입니다. IHitInterface의 GetHit 함수를 오버라이딩합니다.
void APhase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// 필살기 모드가 아니면 부모 클래스의 GetHit_Implementation 함수를 호출합니다.
	if (LethalState == ELethalState::ELS_Off) {
		Super::GetHit_Implementation(ImpactPoint, Hitter);
	}
	// 필살기 모드일 경우, 아무런 처리를 하지 않습니다.
	else if (LethalState == ELethalState::ELS_On) {
		return;
	}
}

// 캐릭터가 공격할 때 호출되는 함수입니다.
void APhase::Attack()
{
	// 공격이 가능한 상태가 아니면 리턴합니다.
	if (!CanAttack()) return;

	// AttackEffect와 무기가 설정되어 있다면, 이펙트를 생성합니다.
	if (AttackEffect && EquippedWeapon) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			AttackEffect,
			EquippedWeapon->GetActorLocation()
		);
	}

	// 부모 클래스의 Attack 함수를 호출합니다.
	Super::Attack();
}

// 필살기 모드를 활성화하는 함수입니다.
void APhase::LethalMode()
{
	// 이미 필살기 모드라면 리턴합니다.
	if (LethalState == ELethalState::ELS_On) return;

	// 부모 클래스의 LethalMode 함수를 호출합니다.
	Super::LethalMode();

	// 필살기 모드가 활성화되면, 소울을 추가하고, 10초 후에 LethalModeFinish 함수를 호출합니다.
	if (LethalState == ELethalState::ELS_On) {
		if (Attributes) {
			Attributes->AddSouls(SoulSpent);
			CombatOverlay->UpdateSoulText(Attributes->GetSouls());
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhase::LethalModeFinish, 10.0f, false);
	}
	else if (LethalState == ELethalState::ELS_Off) return;
}

// 필살기 모드를 종료하는 함수입니다.
void APhase::LethalModeFinish()
{
	Super::LethalModeFinish();  // 부모 클래스의 LethalModeFinish를 호출합니다.
}
