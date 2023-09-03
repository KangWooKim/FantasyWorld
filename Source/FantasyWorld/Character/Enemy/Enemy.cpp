// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "AIController.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "FantasyWorld/HUD/HealthBarComponent.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "FantasyWorld/Pickups/Soul.h"
#include "Kismet/GameplayStatics.h"



// �⺻ ������ �Լ�
AEnemy::AEnemy()
{
	// �� ���Ͱ� �� �����Ӹ��� Tick �Լ��� ȣ���ϵ��� �����մϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// ������ �޽��� ���õ� �浹 ������ �ʱ�ȭ�մϴ�.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// HealthBarWidget�� �� ������ ��Ʈ ������Ʈ�� ���Դϴ�.
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// ĳ���� �̵��� ȸ���� ���� �����Դϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// PawnSensing ������Ʈ�� �ʱ�ȭ�մϴ�.
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

// �� �����Ӹ��� ȣ��Ǵ� �Լ�
void AEnemy::Tick(float DeltaTime)
{
	// �θ� Ŭ������ Tick �Լ��� ȣ���մϴ�.
	Super::Tick(DeltaTime);

	// EnemyController�� �ʱ�ȭ�մϴ�.
	EnemyController = EnemyController == nullptr ? Cast<AAIController>(GetController()) : EnemyController;

	// ���� �׾����� �ƹ� �۾��� ���� �ʽ��ϴ�.
	if (IsDead()) return;

	// ���� ���¿� ���� Ÿ���� Ȯ���մϴ�.
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

// �������� ó���ϴ� �Լ�
// DamageAmount: ���� ������ ��
// DamageEvent: ������ �̺�Ʈ ����
// EventInstigator: �������� �� ��Ʈ�ѷ�
// DamageCauser: �������� �� ����
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// �̺�Ʈ�� ����Ų ��Ʈ�ѷ��� ���Ͱ� ������ �������� �״�� ��ȯ�մϴ�.
	if (!EventInstigator || !DamageCauser) return DamageAmount;

	// �������� ó���մϴ�.
	HandleDamage(DamageAmount);

	// ���� ����� �����մϴ�.
	CombatTarget = EventInstigator->GetPawn();

	// ���� ���� ���� ������ ���� ���·� ��ȯ�մϴ�.
	if (IsInsideAttackRadius()) {
		EnemyState = EEnemyState::EES_Attacking;
	}
	// ���� ���� ���̸� Ÿ���� �߰��մϴ�.
	else if (IsOutsideAttackRadius()) {
		ChaseTarget();
	}

	return DamageAmount;
}

// �� ���Ͱ� �ı��� �� ȣ��Ǵ� �Լ�
void AEnemy::Destroyed()
{
	// ������ ���⸦ �ı��մϴ�.
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	// �� ��° ���⵵ �ִٸ� �ı��մϴ�.
	if (EquippedWeaponSecond) {
		EquippedWeaponSecond->Destroy();
	}
}


// ���Ͱ� ������ �޾��� ���� �����Դϴ�.
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// �θ� Ŭ������ GetHit_Implementation �Լ��� ȣ���մϴ�.
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	// ���� ���� �ʾҴٸ� ü�� �ٸ� ǥ���մϴ�.
	if (!IsDead()) ShowHealthBar();

	// ������ ���� Ÿ�̸Ӹ� �ʱ�ȭ�մϴ�.
	ClearPatrolTimer();
	ClearAttackTimer();

	// ���� �浹�� ��Ȱ��ȭ�մϴ�.
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// ���� ����� �ߴ��մϴ�.
	StopAttackMontage();

	// ���� ���� �ȿ� �ִٸ�, ���� Ÿ�̸Ӹ� �����մϴ�.
	if (IsInsideAttackRadius()) {
		if (!IsDead()) {
			StartAttackTimer();
		}
	}
}

// ������ ���۵ǰų� �� ���Ͱ� ������ �� ȣ��˴ϴ�.
void AEnemy::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay �Լ��� ȣ���մϴ�.
	Super::BeginPlay();

	// PawnSensing ������Ʈ�� �ִٸ� PawnSeen �Լ��� ���ε��մϴ�.
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	// ���� �ʱ�ȭ�մϴ�.
	InitializeEnemy();

	// �� ���Ϳ� "Enemy" �±׸� �߰��մϴ�.
	Tags.Add(FName("Enemy"));
}

// ���� �״� ����� �����Դϴ�.
void AEnemy::Die_Implementation()
{
	// �θ� Ŭ������ Die_Implementation �Լ��� ȣ���մϴ�.
	Super::Die_Implementation();

	// ���� ���¸� Dead�� �����մϴ�.
	EnemyState = EEnemyState::EES_Dead;

	// ���� Ÿ�̸ӿ� ü�� �ٸ� ����ϴ�.
	ClearAttackTimer();
	HideHealthBar();

	// ĸ�� ������Ʈ�� ��Ȱ��ȭ�մϴ�.
	DisableCapsule();

	// �� ������ ������ �����մϴ�.
	SetLifeSpan(DeathLifeSpan);

	// ĳ������ �̵� ����� ȸ���� ����ȭ���� �ʵ��� �����մϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// ������ �浹�� ��Ȱ��ȭ�մϴ�.
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// �ҿ��� �����մϴ�.
	SpawnSoul();
}

// �ҿ��� �����ϴ� �Լ��Դϴ�.
void AEnemy::SpawnSoul()
{
	// �ҿ� Ŭ������ �Ӽ��� �ִٸ� �ҿ��� �����մϴ�.
	UWorld* World = GetWorld();
	if (World && SoulClass && Attributes) {
		const FVector SpawnLocation = GetActorLocation() + FVector(0.f, 0.f, 125.f);
		ASoul* SpawnedSoul = World->SpawnActor<ASoul>(SoulClass, SpawnLocation, GetActorRotation());
		if (SpawnedSoul) {
			SpawnedSoul->SetSouls(Attributes->GetSouls());
			SpawnedSoul->SetOwner(this);
		}
	}
}

// ���� �����ϴ� �Լ��Դϴ�.
void AEnemy::Attack()
{
	// �θ� Ŭ������ Attack �Լ��� ȣ���մϴ�.
	Super::Attack();

	// ���� ����� ���ٸ� �Լ��� �������ɴϴ�.
	if (CombatTarget == nullptr) return;

	// ���� ���¸� Engaged�� �����մϴ�.
	EnemyState = EEnemyState::EES_Engaged;

	// ���� ����� �����մϴ�.
	PlayAttackMontage();
}

// ���� ������ �� �ִ��� Ȯ���ϴ� �Լ��Դϴ�.
bool AEnemy::CanAttack()
{
	// ���� ������ ���ǵ��� �˻��մϴ�.
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

// ���� ������ ��ģ �� ȣ��Ǵ� �Լ��Դϴ�.
void AEnemy::AttackEnd()
{
	// ���� ���¸� �ʱ�ȭ�մϴ�.
	EnemyState = EEnemyState::EES_NoState;

	// ���� ����� �ٽ� Ȯ���մϴ�.
	CheckCombatTarget();
}

// ���� �������� �޾��� ���� ó���� �մϴ�.
void AEnemy::HandleDamage(float DamageAmount)
{
	// �θ� Ŭ������ HandleDamage �Լ��� ȣ���մϴ�.
	Super::HandleDamage(DamageAmount);

	// ü�� �ٿ� �Ӽ��� �ִٸ� ü�� �ٸ� ������Ʈ�մϴ�.
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}



// ���� �ʱ�ȭ�ϴ� �Լ��Դϴ�.
void AEnemy::InitializeEnemy()
{
	// ���� ��Ʈ�ѷ��� AAIController�� ĳ�����մϴ�.
	EnemyController = Cast<AAIController>(GetController());

	// ���� ������ �̵��� �����մϴ�.
	MoveToTarget(PatrolTarget);

	// ü�� �ٸ� ����ϴ�.
	HideHealthBar();

	// �⺻ ���⸦ �����մϴ�.
	SpawnDefaultWeapon();
}

// ���� ����� Ȯ���ϴ� �Լ��Դϴ�.
void AEnemy::CheckPatrolTarget()
{
	// ���� ������ �Ÿ��� PatrolRadius �̳����� Ȯ���մϴ�.
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		// ���ο� ���� ����� �����մϴ�.
		PatrolTarget = ChoosePatrolTarget();

		// ���� ��� �ð��� �������� �����մϴ�.
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);

		// ������ ���� �� ȣ���� �Լ��� ��� �ð��� �����մϴ�.
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

// ���� ����� Ȯ���ϴ� �Լ��Դϴ�.
void AEnemy::CheckCombatTarget()
{
	// ���� �ݰ� �ۿ� �ִ��� Ȯ���մϴ�.
	if (IsOutsideCombatRadius())
	{
		// ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϰ� ������ �ҽ��ϴ�.
		ClearAttackTimer();
		LoseInterest();

		// ���� ���°� �ƴϸ� ������ �����մϴ�.
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		// ���� Ÿ�̸Ӹ� �ʱ�ȭ�մϴ�.
		ClearAttackTimer();

		// ���� ���°� �ƴϸ� ����� �߰��մϴ�.
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		// ���� Ÿ�̸Ӹ� �����մϴ�.
		StartAttackTimer();
	}
}

// ���� Ÿ�̸Ӱ� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
void AEnemy::PatrolTimerFinished()
{
	// �ٽ� ���� ������ �̵��� �����մϴ�.
	MoveToTarget(PatrolTarget);
}

// ü�� �ٸ� ����� �Լ��Դϴ�.
void AEnemy::HideHealthBar()
{
	// HealthBarWidget�� �����ϸ� ü�� �ٸ� ����ϴ�.
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

// ü�� �ٸ� ǥ���ϴ� �Լ��Դϴ�.
void AEnemy::ShowHealthBar()
{
	// HealthBarWidget�� �����ϸ� ü�� �ٸ� ǥ���մϴ�.
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

// ���� ���� ��� ���� ������ �Ҵ� �Լ��Դϴ�.
void AEnemy::LoseInterest()
{
	// ���� ����� null�� �����ϰ� ü�� �ٸ� ����ϴ�.
	CombatTarget = nullptr;
	HideHealthBar();
}

// ������ �����ϴ� �Լ��Դϴ�.
void AEnemy::StartPatrolling()
{
	// ���� ���¸� ������ �����մϴ�.
	EnemyState = EEnemyState::EES_Patrolling;

	// ĳ������ �̵� �ӵ��� ���� �ӵ��� �����մϴ�.
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;

	// ���� ������ �̵��� �����մϴ�.
	MoveToTarget(PatrolTarget);
}

// ���� ����� �߰��ϴ� �Լ��Դϴ�.
void AEnemy::ChaseTarget()
{
	// ���� ���¸� �߰����� �����մϴ�.
	EnemyState = EEnemyState::EES_Chasing;

	// ĳ������ �̵� �ӵ��� �߰� �ӵ��� �����մϴ�.
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;

	// ���� ������ �̵��� �����մϴ�.
	MoveToTarget(CombatTarget);

	// �߰� ���尡 �����Ǿ� �ִٸ� ���带 ����մϴ�.
	if (ChaseSound) {
		UGameplayStatics::PlaySoundAtLocation(this, ChaseSound, this->GetActorLocation());
	}
}


// ���� �ݰ� �ۿ� �ִ��� Ȯ���ϴ� �޼���
bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);  // ���� �ݰ� ���� ������ true ��ȯ
}

// ���� �ݰ� �ۿ� �ִ��� Ȯ���ϴ� �޼���
bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);  // ���� �ݰ� ���� ������ true ��ȯ
}

// ���� �ݰ� �ȿ� �ִ��� Ȯ���ϴ� �޼���
bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);  // ���� �ݰ� ���� ������ true ��ȯ
}

// ���� ���� ������ Ȯ���ϴ� �޼���
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;  // ���� ���°� '���� ��'�̸� true ��ȯ
}

// ���� ���� ������ Ȯ���ϴ� �޼���
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;  // ���� ���°� '���� ��'�̸� true ��ȯ
}

// ���� �׾����� Ȯ���ϴ� �޼���
bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;  // ���� ���°� '����'�̸� true ��ȯ
}

// ���� ���� ������ Ȯ���ϴ� �޼���
bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;  // ���� ���°� '���� ��'�̸� true ��ȯ
}

// ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϴ� �޼���
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);  // ���� Ÿ�̸Ӹ� �ʱ�ȭ
}

// ���� Ÿ�̸Ӹ� �����ϴ� �޼���
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;  // ���� ���¸� '���� ��'���� ����
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);  // ������ ���� �ð��� ����
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);  // ���� Ÿ�̸Ӹ� ����
}

// ���� Ÿ�̸Ӹ� �ʱ�ȭ�ϴ� �޼���
void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);  // ���� Ÿ�̸Ӹ� �ʱ�ȭ
}

// ����� Ư�� �ݰ� ���� �ִ��� Ȯ���ϴ� �޼���. Target�� ��� ��ü, Radius�� �ݰ�
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;  // ����� ������ false ��ȯ
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();  // ������ �Ÿ��� ���
	return DistanceToTarget <= Radius;  // �Ÿ��� �ݰ� ���� ������ true ��ȯ
}

// ������ �̵��ϴ� �޼���. Target�� �̵��� ���
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;  // ��Ʈ�ѷ��� ����� ������ ����
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);  // �̵��� ��� ����
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);  // �ν� �ݰ� ����
	EnemyController->MoveTo(MoveRequest);  // �̵� ��� ����
}

// ���� ����� �����ϴ� �޼���
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)  // ��� ���� ����� ��ȸ
	{
		if (Target != PatrolTarget)  // ���� ���� ����� �ƴϸ�
		{
			ValidTargets.AddUnique(Target);  // ��ȿ�� ��� �迭�� �߰�
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();  // ��ȿ�� ���� ����� ��
	if (NumPatrolTargets > 0)  // ��ȿ�� ����� ������
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);  // ������ ����� ����
		return ValidTargets[TargetSelection];  // ������ ����� ��ȯ
	}
	return nullptr;  // ��ȿ�� ����� ������ nullptr ��ȯ
}

// �⺻ ���⸦ �����ϴ� �޼���
void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();  // ���� ���带 ������
	if (World && WeaponClass)  // ����� ���� Ŭ������ ������
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);  // ���⸦ ����
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket1"), this, this);  // ���� ����
		EquippedWeapon = DefaultWeapon;  // ������ ���⸦ ����
	}

	// �� ��° ���� ������ �ִ� ���
	if (World && WeaponClass && GetMesh()->DoesSocketExist("WeaponSocket2")) {
		AWeapon* DefaultWeaponSecond = World->SpawnActor<AWeapon>(WeaponClass);  // �� ��° ���⸦ ����
		DefaultWeaponSecond->Equip(GetMesh(), FName("WeaponSocket2"), this, this);  // �� ��° ���� ����
		EquippedWeaponSecond = DefaultWeaponSecond;  // ������ �� ��° ���⸦ ����
	}
	else {
		EquippedWeaponSecond = nullptr;  // �� ��° ���� ������ ������ nullptr ����
	}
}

// ���� �� �� ȣ��Ǵ� �޼���
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&  // ���� �ʾҰ�
		EnemyState != EEnemyState::EES_Chasing &&  // �̹� ���� ������ �ʰ�
		EnemyState < EEnemyState::EES_Attacking&&  // ���� ���� �ƴϰ�
		SeenPawn->ActorHasTag(FName("EngageableTarget"));  // �� ���� ���� ������ ����̶��

	if (bShouldChaseTarget)  // �����ؾ� �Ѵٸ�
	{
		CombatTarget = SeenPawn;  // ���� ����� ����
		ClearPatrolTimer();  // ���� Ÿ�̸Ӹ� �ʱ�ȭ
		ChaseTarget();  // ����� ����
	}
}
