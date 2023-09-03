// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// ABaseEnemy 클래스의 생성자 구현
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;  // 이 액터는 틱을 받을 수 있음

	// "Attributes" 라는 이름으로 UAttributeComponent 생성
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// 캡슐 컴포넌트의 카메라 채널에 대한 충돌 응답을 무시로 설정
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// 게임이 시작될 때 실행되는 BeginPlay 함수의 구현
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();  // 부모 클래스의 BeginPlay 호출
	Tags.AddUnique(FName("Enemy"));  // "Enemy" 태그 추가
}

// IHitInterface에 정의된 GetHit 함수의 구현
void ABaseEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// 적이 살아있고, Hitter가 nullptr이 아닌 경우
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());  // 방향성 있는 히트 반응 실행
		PlayHitSound(ImpactPoint);  // 히트 사운드 재생
	}
	else {
		Die();  // 죽음 처리
	}

	// 히트 파티클 스폰
	SpawnHitParticles(ImpactPoint);
}

// 공격 함수
void ABaseEnemy::Attack()
{
	// 타겟이 존재하고, "Dead" 태그가 있다면 타겟을 null로 설정
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
	}
}

// 죽는 함수의 구현
void ABaseEnemy::Die_Implementation()
{
	// DeathSound가 있다면 DeathSound 재생
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation());
	}

	// "Dead" 태그 추가
	Tags.Add(FName("Dead"));

	// 죽는 모션 재생
	PlayDeathMontage();

	// 죽는 이벤트 브로드캐스트
	EnemyDeath.Broadcast();
}

// HitReactMontage를 재생하는 함수
void ABaseEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();  // 애니메이션 인스턴스 가져오기

	// 애니메이션 인스턴스와 HitReactMontage가 있다면 Montage를 재생
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);  // Montage 재생
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);  // 특정 섹션으로 점프
	}
}


// DirectionalHitReact 메서드는 적이 어느 방향에서 공격을 받았는지를 판단하고, 해당 방향에 따라 반응 애니메이션을 재생합니다.
// ImpactPoint: 공격이 맞은 위치의 좌표
void ABaseEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	// 적의 전방향을 가져옵니다.
	const FVector Forward = GetActorForwardVector();
	// 충돌 지점의 Z 좌표를 적의 Z 좌표로 변경합니다.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	// 적과 충돌 지점 사이의 정규화된 벡터를 계산합니다.
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// 적과 충돌 지점 사이의 각도를 계산합니다.
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// 각도의 부호를 결정합니다.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	// 반응 애니메이션의 구간을 결정합니다.
	FName Section("FromBack");
	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	// 반응 애니메이션을 재생합니다.
	PlayHitReactMontage(Section);
}

// PlayHitSound 메서드는 적이 공격을 받았을 때의 소리를 재생합니다.
// ImpactPoint: 공격이 맞은 위치의 좌표
void ABaseEnemy::PlayHitSound(const FVector& ImpactPoint)
{
	// 소리가 설정되어 있다면 소리를 재생합니다.
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

// SpawnHitParticles 메서드는 적이 공격을 받았을 때의 입자 효과를 생성합니다.
// ImpactPoint: 공격이 맞은 위치의 좌표
void ABaseEnemy::SpawnHitParticles(const FVector& ImpactPoint)
{
	// 입자 효과가 설정되어 있고 월드가 유효하다면 입자 효과를 생성합니다.
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

// HandleDamage 메서드는 적이 데미지를 받았을 때의 처리를 담당합니다.
// DamageAmount: 받은 데미지 양
void ABaseEnemy::HandleDamage(float DamageAmount)
{
	// 속성이 설정되어 있다면 데미지를 적용합니다.
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

// PlayMontageSection 메서드는 지정된 애니메이션 몽타주의 특정 구간을 재생합니다.
// Montage: 재생할 애니메이션 몽타주
// SectionName: 재생할 몽타주 구간의 이름
void ABaseEnemy::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	// 애니메이션 인스턴스와 몽타주가 유효하다면 애니메이션을 재생합니다.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}


// PlayRandomMontageSection 메서드는 주어진 몽타주(Montage)에서 랜덤하게 섹션을 선택하여 재생합니다.
// Montage: 재생할 애니메이션 몽타주
// SectionNames: 선택할 수 있는 몽타주 섹션의 이름 목록
// 반환 값: 선택된 섹션의 인덱스. 선택할 섹션 이름이 없으면 -1을 반환합니다.
int32 ABaseEnemy::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	// 섹션 이름이 없으면 -1을 반환합니다.
	if (SectionNames.Num() <= 0) return -1;
	// 랜덤 섹션을 선택하기 위한 범위를 설정합니다.
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	// 랜덤으로 선택한 섹션을 재생합니다.
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

// PlayAttackMontage 메서드는 공격 애니메이션 몽타주를 랜덤 섹션으로 재생합니다.
// 반환 값: 선택된 섹션의 인덱스
int32 ABaseEnemy::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

// PlayDeathMontage 메서드는 죽음 애니메이션 몽타주를 랜덤 섹션으로 재생합니다.
// 반환 값: 선택된 섹션의 인덱스
int32 ABaseEnemy::PlayDeathMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("deathMontage"))
		// 랜덤 섹션을 선택하여 재생합니다.
		const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	// 선택된 섹션에 따라 죽음 포즈를 설정합니다.
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

// PlayDodgeMontage 메서드는 회피 애니메이션 몽타주를 재생합니다.
// "Default" 섹션을 사용합니다.
void ABaseEnemy::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

// StopAttackMontage 메서드는 공격 애니메이션 몽타주를 중단합니다.
void ABaseEnemy::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// 애니메이션 인스턴스가 유효하면 몽타주를 중단합니다.
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

// GetTranslationWarpTarget 메서드는 이동할 대상 위치를 계산하여 반환합니다.
// 반환 값: 이동할 대상 위치의 좌표
FVector ABaseEnemy::GetTranslationWarpTarget()
{
	// 대상이 없으면 빈 벡터를 반환합니다.
	if (CombatTarget == nullptr) return FVector();
	// 대상과 자신의 위치를 가져옵니다.
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	// 대상과 자신 사이의 방향을 계산합니다.
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	// 이동할 대상 위치를 반환합니다.
	return CombatTargetLocation + TargetToMe;
}

// GetRotationWarpTarget 메서드는 회전할 대상 위치를 반환합니다.
// 반환 값: 회전할 대상 위치의 좌표
FVector ABaseEnemy::GetRotationWarpTarget()
{
	// 대상이 있다면 그 위치를 반환하고, 없으면 빈 벡터를 반환합니다.
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

// DisableCapsule 메서드는 캡슐 컴포넌트의 충돌을 비활성화합니다.
void ABaseEnemy::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// CanAttack 메서드는 적이 공격할 수 있는지 여부를 반환합니다.
// 반환 값: 공격 가능 여부 (이 경우 false)
bool ABaseEnemy::CanAttack()
{
	return false;
}


// IsAlive 메서드는 적(Enemy)이 살아있는지 여부를 반환합니다.
// 반환 값: 적이 살아있으면 true, 죽었으면 false
bool ABaseEnemy::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

// DisableMeshCollision 메서드는 메시의 충돌을 비활성화합니다.
void ABaseEnemy::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// AttackEnd 메서드는 공격이 끝났을 때 호출되는 함수입니다.
// 현재는 구현되지 않았습니다.
void ABaseEnemy::AttackEnd()
{
}

// DodgeEnd 메서드는 회피가 끝났을 때 호출되는 함수입니다.
// 현재는 구현되지 않았습니다.
void ABaseEnemy::DodgeEnd()
{

}

// Tick 메서드는 매 프레임마다 호출되는 업데이트 함수입니다.
// DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
void ABaseEnemy::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick 함수를 호출합니다.
	Super::Tick(DeltaTime);
}

// SetWeaponCollisionEnabled 메서드는 무기의 충돌 여부를 설정합니다.
// CollisionEnabled: 설정할 충돌 여부 (활성화/비활성화)
void ABaseEnemy::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	// 첫 번째 무기가 있다면 충돌 설정을 변경합니다.
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		// 무시할 액터 목록을 초기화합니다.
		EquippedWeapon->IgnoreActors.Empty();
	}
	// 두 번째 무기가 있다면 충돌 설정을 변경합니다.
	if (EquippedWeaponSecond && EquippedWeaponSecond->GetWeaponBox())
	{
		EquippedWeaponSecond->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		// 무시할 액터 목록을 초기화합니다.
		EquippedWeaponSecond->IgnoreActors.Empty();
	}
}
