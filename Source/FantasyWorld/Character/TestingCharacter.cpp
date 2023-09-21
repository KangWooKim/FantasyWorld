// Fill out your copyright notice in the Description page of Project Settings.


#include "TestingCharacter.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystem.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/Character/Enemy/Enemy.h"

ATestingCharacter::ATestingCharacter()
{
}

void ATestingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ATestingCharacter::Attack);
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &ATestingCharacter::LethalMode);
}

void ATestingCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ATestingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestingCharacter::Attack()
{
	Super::Attack();
}

void ATestingCharacter::LethalMode()
{
	Super::LethalMode();

	if (LethalState == ELethalState::ELS_Off) {
		return;
	}

	Attributes->AddSouls(SoulSpent);
	if (CombatOverlay) {
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}

	LethalModeTarget.Empty();
	UGameplayStatics::GetAllActorsOfClass(this, AEnemy::StaticClass(), LethalModeTarget);

	for (AActor* E : LethalModeTarget) {
		AEnemy* Target = Cast<AEnemy>(E);
		Target->SetActorTickEnabled(false);
	}

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ATestingCharacter::LethalModeFinish, 5.0f, false);

}

void ATestingCharacter::LethalModeFinish()
{
	for (AActor* E : LethalModeTarget) {
		AEnemy* Target = Cast<AEnemy>(E);
		if (Target) {
			Target->SetActorTickEnabled(true);
		}
	}

	LethalModeTarget.Empty();

	Super::LethalModeFinish();
}
