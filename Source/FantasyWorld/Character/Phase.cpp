// Fill out your copyright notice in the Description page of Project Settings.


#include "Phase.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy/Enemy.h"

APhase::APhase()
{
}

void APhase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &APhase::Attack);
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &APhase::LethalMode);
}

float APhase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (LethalState == ELethalState::ELS_Off) {
		Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	else if (LethalState == ELethalState::ELS_On) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ShieldEffect, this->GetActorLocation(), this->GetActorRotation());
		UGameplayStatics::ApplyDamage(DamageCauser->GetOwner(), DamageAmount, GetWorld()->GetFirstPlayerController(), this, UDamageType::StaticClass());
		IHitInterface* HitInterface = Cast<IHitInterface>(DamageCauser->GetOwner());
		if (HitInterface)
		{
			HitInterface->Execute_GetHit(DamageCauser->GetOwner(), DamageCauser->GetOwner()->GetActorLocation(), this);
		}
		Attributes->UseStamina(DamageAmount);
		CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
	}
	return DamageAmount;
}

void APhase::BeginPlay()
{
	Super::BeginPlay();
}

void APhase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhase::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (LethalState == ELethalState::ELS_Off) {
		Super::GetHit_Implementation(ImpactPoint, Hitter);
	}
	else if (LethalState == ELethalState::ELS_On) {
		return;
	}
}

void APhase::Attack()
{
	if (!CanAttack()) return;
	if (AttackEffect && EquippedWeapon) {
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			AttackEffect,
			EquippedWeapon->GetActorLocation()
		);
	}
	Super::Attack();
}

void APhase::LethalMode()
{
	if (LethalState == ELethalState::ELS_On) return;

	Super::LethalMode();
	if (LethalState == ELethalState::ELS_On) {
		if (Attributes) {
			Attributes->AddSouls(SoulSpent);
			CombatOverlay->UpdateSoulText(Attributes->GetSouls());
		}

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhase::LethalModeFinish, 10.0f, false);
	}

	else if(LethalState == ELethalState::ELS_Off) return;
}

void APhase::LethalModeFinish()
{
	Super::LethalModeFinish();
}

