// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy.h"

// APhase Ŭ������ �⺻ �������Դϴ�.
APhase::APhase()
{
}

// �Է� ������Ʈ�� �����ϴ� �Լ��Դϴ�. "Attack"�� "LethalMode" �׼��� ���ε��մϴ�.
void APhase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);  // �θ� Ŭ������ �Լ��� ȣ���մϴ�.

	// "Attack"�̶�� �̸��� �׼��� ������ APhase::Attack �Լ��� ȣ��˴ϴ�.
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &APhase::Attack);

	// "LethalMode"�̶�� �̸��� �׼��� ������ APhase::LethalMode �Լ��� ȣ��˴ϴ�.
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &APhase::LethalMode);
}

// ĳ���Ͱ� �������� ���� �� ȣ��Ǵ� �Լ��Դϴ�.
float APhase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// �ʻ�� ��尡 �ƴϸ� �θ� Ŭ������ TakeDamage �Լ��� ȣ���մϴ�.
	if (LethalState == ELethalState::ELS_Off) {
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	// �ʻ�� ����� ���, ShieldEffect�� �����ϰ� ���¹̳��� ����մϴ�.
	else if (LethalState == ELethalState::ELS_On) {
		// ���� ����Ʈ ����
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldEffect, this->GetActorLocation(), this->GetActorRotation());

		// �������� ���� ���Ϳ��� ������ �������� �����ϴ�.
		UGameplayStatics::ApplyDamage(DamageCauser->GetOwner(), DamageAmount, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());

		// �������� ���� ���Ͱ� IHitInterface�� �����ϰ� �ִٸ�, GetHit �Լ��� ȣ���մϴ�.
		IHitInterface* HitInterface = Cast<IHitInterface>(DamageCauser->GetOwner());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(DamageCauser->GetOwner(), DamageCauser->GetOwner()->GetActorLocation(), this);
		}

		// ���¹̳��� �Ҹ��մϴ�.
		Attributes->UseStamina(DamageAmount);

		// ���¹̳� �� UI�� ������Ʈ�մϴ�.
		CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
	}
	return DamageAmount;
}

// ������ ���۵� �� �� ���� ȣ��˴ϴ�.
void APhase::BeginPlay()
{
	Super::BeginPlay();  // �θ� Ŭ������ BeginPlay�� ȣ���մϴ�.
}

// �����Ӹ��� ȣ��Ǵ� �Լ��Դϴ�.
void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  // �θ� Ŭ������ Tick �Լ��� ȣ���մϴ�.
}

// ĳ���Ͱ� �������� ���� �� ȣ��Ǵ� �Լ��Դϴ�. IHitInterface�� GetHit �Լ��� �������̵��մϴ�.
void APhase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// �ʻ�� ��尡 �ƴϸ� �θ� Ŭ������ GetHit_Implementation �Լ��� ȣ���մϴ�.
	if (LethalState == ELethalState::ELS_Off) {
		Super::GetHit_Implementation(ImpactPoint, Hitter);
	}
	// �ʻ�� ����� ���, �ƹ��� ó���� ���� �ʽ��ϴ�.
	else if (LethalState == ELethalState::ELS_On) {
		return;
	}
}

// ĳ���Ͱ� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
void APhase::Attack()
{
	// ������ ������ ���°� �ƴϸ� �����մϴ�.
	if (!CanAttack()) return;

	// AttackEffect�� ���Ⱑ �����Ǿ� �ִٸ�, ����Ʈ�� �����մϴ�.
	if (AttackEffect && EquippedWeapon) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			AttackEffect,
			EquippedWeapon->GetActorLocation()
		);
	}

	// �θ� Ŭ������ Attack �Լ��� ȣ���մϴ�.
	Super::Attack();
}

// �ʻ�� ��带 Ȱ��ȭ�ϴ� �Լ��Դϴ�.
void APhase::LethalMode()
{
	// �̹� �ʻ�� ����� �����մϴ�.
	if (LethalState == ELethalState::ELS_On) return;

	// �θ� Ŭ������ LethalMode �Լ��� ȣ���մϴ�.
	Super::LethalMode();

	// �ʻ�� ��尡 Ȱ��ȭ�Ǹ�, �ҿ��� �߰��ϰ�, 10�� �Ŀ� LethalModeFinish �Լ��� ȣ���մϴ�.
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

// �ʻ�� ��带 �����ϴ� �Լ��Դϴ�.
void APhase::LethalModeFinish()
{
	Super::LethalModeFinish();  // �θ� Ŭ������ LethalModeFinish�� ȣ���մϴ�.
}
