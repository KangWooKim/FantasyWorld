// Fill out your copyright notice in the Description page of Project Settings.


#include "EliteZombie.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/DamageType.h"
#include "Engine/EngineTypes.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"

AEliteZombie::AEliteZombie()
{
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ��ũ ������ Ȱ��ȭ
	SetReplicates(true);
	SetReplicateMovement(true);

	PatrolRadius = 1000.0f;
	TimeToSleep = 10.0f;

	Health = 1000000.0f;
	AttackRange = 80.0f;
	Damage = 50.f;
	AttackCooldown = 3.f;

	LegDamageAccumulated = 0.f;
}

void AEliteZombie::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = 600.f;
}

float AEliteZombie::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (DamageCauser && DamageCauser->IsA(ABaseZombie::StaticClass()))
	{
		return 0.0f;
	}

	if (ZombieState == EZombieState::Death)
	{
		return 0.0f;
	}

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;

	if (Health <= 0)
	{
		HandleDeath();
		return ActualDamage;
	}

	// 'Hit reaction' ���·� ��ȯ
	if (ZombieState != EZombieState::HitReact && ZombieInjuryState != EZombieInjuryState::Injured)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_None);
		ZombieState = EZombieState::HitReact;

		// ���� Ÿ�̸Ӱ� ������ ���
		GetWorldTimerManager().ClearTimer(TimerHandle_HitReactEnd);

		// 'hit reaction' ���¸� ���� �ð�(��: 3��) �Ŀ� �����ϵ��� Ÿ�̸� ����
		GetWorldTimerManager().SetTimer(TimerHandle_HitReactEnd, this, &AEliteZombie::HitReactEnd, 1.0f, false);
	}

	return ActualDamage;
}

void AEliteZombie::GetDown()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	ZombieInjuryState = EZombieInjuryState::Down;

	GetCharacterMovement()->MaxWalkSpeed = 100.f;
}

void AEliteZombie::DownEnd()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	ZombieInjuryState = EZombieInjuryState::Injured;

	FTimerHandle TimerHandle_StandUp;

	// 60�� �� StandUp �޼��� ȣ�� ����
	GetWorldTimerManager().SetTimer(TimerHandle_StandUp, this, &AEliteZombie::StandUp, 60.0f, false);
}

void AEliteZombie::StandUp()
{
	GetCharacterMovement()->SetMovementMode(MOVE_None);

	ZombieInjuryState = EZombieInjuryState::Stand;
}

void AEliteZombie::StandUpEnd()
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	ZombieInjuryState = EZombieInjuryState::None;
	ZombieState = EZombieState::None;

	LegDamageAccumulated = 0.f;

	GetCharacterMovement()->MaxWalkSpeed = 600.f;

}

void AEliteZombie::Attack(APawn* Target)
{
	Super::Attack(Target);
	
}

void AEliteZombie::ApplyDamageToTarget()
{
	Super::ApplyDamageToTarget();
}

void AEliteZombie::TakeShot(FHitResultData HitResult, float WeaponDamage)
{
	Super::TakeShot(HitResult, WeaponDamage);

	if (HitResult.bHitLeg)
	{
		LegDamageAccumulated += WeaponDamage;
	}

	if (ZombieInjuryState == EZombieInjuryState::None && LegDamageAccumulated >= 100)
	{
		GetDown();
	}
}

void AEliteZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEliteZombie, ZombieInjuryState);
	DOREPLIFETIME(AEliteZombie, LegDamageAccumulated);
}
