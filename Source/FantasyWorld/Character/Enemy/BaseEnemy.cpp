// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"


// ABaseEnemy Ŭ������ ������ ����
ABaseEnemy::ABaseEnemy()
{
	PrimaryActorTick.bCanEverTick = true;  // �� ���ʹ� ƽ�� ���� �� ����

	// "Attributes" ��� �̸����� UAttributeComponent ����
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// ĸ�� ������Ʈ�� ī�޶� ä�ο� ���� �浹 ������ ���÷� ����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// ������ ���۵� �� ����Ǵ� BeginPlay �Լ��� ����
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();  // �θ� Ŭ������ BeginPlay ȣ��
	Tags.AddUnique(FName("Enemy"));  // "Enemy" �±� �߰�
}

// IHitInterface�� ���ǵ� GetHit �Լ��� ����
void ABaseEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// ���� ����ְ�, Hitter�� nullptr�� �ƴ� ���
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());  // ���⼺ �ִ� ��Ʈ ���� ����
		PlayHitSound(ImpactPoint);  // ��Ʈ ���� ���
	}
	else {
		Die();  // ���� ó��
	}

	// ��Ʈ ��ƼŬ ����
	SpawnHitParticles(ImpactPoint);
}

// ���� �Լ�
void ABaseEnemy::Attack()
{
	// Ÿ���� �����ϰ�, "Dead" �±װ� �ִٸ� Ÿ���� null�� ����
	if (CombatTarget && CombatTarget->ActorHasTag(FName("Dead"))) {
		CombatTarget = nullptr;
	}
}

// �״� �Լ��� ����
void ABaseEnemy::Die_Implementation()
{
	// DeathSound�� �ִٸ� DeathSound ���
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation());
	}

	// "Dead" �±� �߰�
	Tags.Add(FName("Dead"));

	// �״� ��� ���
	PlayDeathMontage();

	// �״� �̺�Ʈ ��ε�ĳ��Ʈ
	EnemyDeath.Broadcast();
}

// HitReactMontage�� ����ϴ� �Լ�
void ABaseEnemy::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();  // �ִϸ��̼� �ν��Ͻ� ��������

	// �ִϸ��̼� �ν��Ͻ��� HitReactMontage�� �ִٸ� Montage�� ���
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);  // Montage ���
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);  // Ư�� �������� ����
	}
}


// DirectionalHitReact �޼���� ���� ��� ���⿡�� ������ �޾Ҵ����� �Ǵ��ϰ�, �ش� ���⿡ ���� ���� �ִϸ��̼��� ����մϴ�.
// ImpactPoint: ������ ���� ��ġ�� ��ǥ
void ABaseEnemy::DirectionalHitReact(const FVector& ImpactPoint)
{
	// ���� �������� �����ɴϴ�.
	const FVector Forward = GetActorForwardVector();
	// �浹 ������ Z ��ǥ�� ���� Z ��ǥ�� �����մϴ�.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	// ���� �浹 ���� ������ ����ȭ�� ���͸� ����մϴ�.
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// ���� �浹 ���� ������ ������ ����մϴ�.
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	// ������ ��ȣ�� �����մϴ�.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	// ���� �ִϸ��̼��� ������ �����մϴ�.
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

	// ���� �ִϸ��̼��� ����մϴ�.
	PlayHitReactMontage(Section);
}

// PlayHitSound �޼���� ���� ������ �޾��� ���� �Ҹ��� ����մϴ�.
// ImpactPoint: ������ ���� ��ġ�� ��ǥ
void ABaseEnemy::PlayHitSound(const FVector& ImpactPoint)
{
	// �Ҹ��� �����Ǿ� �ִٸ� �Ҹ��� ����մϴ�.
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
		);
	}
}

// SpawnHitParticles �޼���� ���� ������ �޾��� ���� ���� ȿ���� �����մϴ�.
// ImpactPoint: ������ ���� ��ġ�� ��ǥ
void ABaseEnemy::SpawnHitParticles(const FVector& ImpactPoint)
{
	// ���� ȿ���� �����Ǿ� �ְ� ���尡 ��ȿ�ϴٸ� ���� ȿ���� �����մϴ�.
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

// HandleDamage �޼���� ���� �������� �޾��� ���� ó���� ����մϴ�.
// DamageAmount: ���� ������ ��
void ABaseEnemy::HandleDamage(float DamageAmount)
{
	// �Ӽ��� �����Ǿ� �ִٸ� �������� �����մϴ�.
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

// PlayMontageSection �޼���� ������ �ִϸ��̼� ��Ÿ���� Ư�� ������ ����մϴ�.
// Montage: ����� �ִϸ��̼� ��Ÿ��
// SectionName: ����� ��Ÿ�� ������ �̸�
void ABaseEnemy::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	// �ִϸ��̼� �ν��Ͻ��� ��Ÿ�ְ� ��ȿ�ϴٸ� �ִϸ��̼��� ����մϴ�.
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}


// PlayRandomMontageSection �޼���� �־��� ��Ÿ��(Montage)���� �����ϰ� ������ �����Ͽ� ����մϴ�.
// Montage: ����� �ִϸ��̼� ��Ÿ��
// SectionNames: ������ �� �ִ� ��Ÿ�� ������ �̸� ���
// ��ȯ ��: ���õ� ������ �ε���. ������ ���� �̸��� ������ -1�� ��ȯ�մϴ�.
int32 ABaseEnemy::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	// ���� �̸��� ������ -1�� ��ȯ�մϴ�.
	if (SectionNames.Num() <= 0) return -1;
	// ���� ������ �����ϱ� ���� ������ �����մϴ�.
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	// �������� ������ ������ ����մϴ�.
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

// PlayAttackMontage �޼���� ���� �ִϸ��̼� ��Ÿ�ָ� ���� �������� ����մϴ�.
// ��ȯ ��: ���õ� ������ �ε���
int32 ABaseEnemy::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

// PlayDeathMontage �޼���� ���� �ִϸ��̼� ��Ÿ�ָ� ���� �������� ����մϴ�.
// ��ȯ ��: ���õ� ������ �ε���
int32 ABaseEnemy::PlayDeathMontage()
{
	UE_LOG(LogTemp, Warning, TEXT("deathMontage"))
		// ���� ������ �����Ͽ� ����մϴ�.
		const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	// ���õ� ���ǿ� ���� ���� ��� �����մϴ�.
	TEnumAsByte<EDeathPose> Pose(Selection);
	if (Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

// PlayDodgeMontage �޼���� ȸ�� �ִϸ��̼� ��Ÿ�ָ� ����մϴ�.
// "Default" ������ ����մϴ�.
void ABaseEnemy::PlayDodgeMontage()
{
	PlayMontageSection(DodgeMontage, FName("Default"));
}

// StopAttackMontage �޼���� ���� �ִϸ��̼� ��Ÿ�ָ� �ߴ��մϴ�.
void ABaseEnemy::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// �ִϸ��̼� �ν��Ͻ��� ��ȿ�ϸ� ��Ÿ�ָ� �ߴ��մϴ�.
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

// GetTranslationWarpTarget �޼���� �̵��� ��� ��ġ�� ����Ͽ� ��ȯ�մϴ�.
// ��ȯ ��: �̵��� ��� ��ġ�� ��ǥ
FVector ABaseEnemy::GetTranslationWarpTarget()
{
	// ����� ������ �� ���͸� ��ȯ�մϴ�.
	if (CombatTarget == nullptr) return FVector();
	// ���� �ڽ��� ��ġ�� �����ɴϴ�.
	const FVector CombatTargetLocation = CombatTarget->GetActorLocation();
	const FVector Location = GetActorLocation();
	// ���� �ڽ� ������ ������ ����մϴ�.
	FVector TargetToMe = (Location - CombatTargetLocation).GetSafeNormal();
	TargetToMe *= WarpTargetDistance;
	// �̵��� ��� ��ġ�� ��ȯ�մϴ�.
	return CombatTargetLocation + TargetToMe;
}

// GetRotationWarpTarget �޼���� ȸ���� ��� ��ġ�� ��ȯ�մϴ�.
// ��ȯ ��: ȸ���� ��� ��ġ�� ��ǥ
FVector ABaseEnemy::GetRotationWarpTarget()
{
	// ����� �ִٸ� �� ��ġ�� ��ȯ�ϰ�, ������ �� ���͸� ��ȯ�մϴ�.
	if (CombatTarget) {
		return CombatTarget->GetActorLocation();
	}
	return FVector();
}

// DisableCapsule �޼���� ĸ�� ������Ʈ�� �浹�� ��Ȱ��ȭ�մϴ�.
void ABaseEnemy::DisableCapsule()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// CanAttack �޼���� ���� ������ �� �ִ��� ���θ� ��ȯ�մϴ�.
// ��ȯ ��: ���� ���� ���� (�� ��� false)
bool ABaseEnemy::CanAttack()
{
	return false;
}


// IsAlive �޼���� ��(Enemy)�� ����ִ��� ���θ� ��ȯ�մϴ�.
// ��ȯ ��: ���� ��������� true, �׾����� false
bool ABaseEnemy::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

// DisableMeshCollision �޼���� �޽��� �浹�� ��Ȱ��ȭ�մϴ�.
void ABaseEnemy::DisableMeshCollision()
{
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// AttackEnd �޼���� ������ ������ �� ȣ��Ǵ� �Լ��Դϴ�.
// ����� �������� �ʾҽ��ϴ�.
void ABaseEnemy::AttackEnd()
{
}

// DodgeEnd �޼���� ȸ�ǰ� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
// ����� �������� �ʾҽ��ϴ�.
void ABaseEnemy::DodgeEnd()
{

}

// Tick �޼���� �� �����Ӹ��� ȣ��Ǵ� ������Ʈ �Լ��Դϴ�.
// DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
void ABaseEnemy::Tick(float DeltaTime)
{
	// �θ� Ŭ������ Tick �Լ��� ȣ���մϴ�.
	Super::Tick(DeltaTime);
}

// SetWeaponCollisionEnabled �޼���� ������ �浹 ���θ� �����մϴ�.
// CollisionEnabled: ������ �浹 ���� (Ȱ��ȭ/��Ȱ��ȭ)
void ABaseEnemy::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	// ù ��° ���Ⱑ �ִٸ� �浹 ������ �����մϴ�.
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		// ������ ���� ����� �ʱ�ȭ�մϴ�.
		EquippedWeapon->IgnoreActors.Empty();
	}
	// �� ��° ���Ⱑ �ִٸ� �浹 ������ �����մϴ�.
	if (EquippedWeaponSecond && EquippedWeaponSecond->GetWeaponBox())
	{
		EquippedWeaponSecond->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		// ������ ���� ����� �ʱ�ȭ�մϴ�.
		EquippedWeaponSecond->IgnoreActors.Empty();
	}
}
