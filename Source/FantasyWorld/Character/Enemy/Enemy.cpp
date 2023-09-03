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



// 기본 생성자 함수
AEnemy::AEnemy()
{
	// 이 액터가 매 프레임마다 Tick 함수를 호출하도록 설정합니다.
	PrimaryActorTick.bCanEverTick = true;

	// 액터의 메쉬와 관련된 충돌 설정을 초기화합니다.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	// HealthBarWidget을 이 액터의 루트 컴포넌트에 붙입니다.
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	// 캐릭터 이동과 회전에 관한 설정입니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// PawnSensing 컴포넌트를 초기화합니다.
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 4000.f;
	PawnSensing->SetPeripheralVisionAngle(45.f);
}

// 매 프레임마다 호출되는 함수
void AEnemy::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick 함수를 호출합니다.
	Super::Tick(DeltaTime);

	// EnemyController를 초기화합니다.
	EnemyController = EnemyController == nullptr ? Cast<AAIController>(GetController()) : EnemyController;

	// 적이 죽었으면 아무 작업도 하지 않습니다.
	if (IsDead()) return;

	// 적의 상태에 따라 타겟을 확인합니다.
	if (EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}

// 데미지를 처리하는 함수
// DamageAmount: 받은 데미지 양
// DamageEvent: 데미지 이벤트 정보
// EventInstigator: 데미지를 준 컨트롤러
// DamageCauser: 데미지를 준 액터
float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// 이벤트를 일으킨 컨트롤러나 액터가 없으면 데미지를 그대로 반환합니다.
	if (!EventInstigator || !DamageCauser) return DamageAmount;

	// 데미지를 처리합니다.
	HandleDamage(DamageAmount);

	// 전투 대상을 설정합니다.
	CombatTarget = EventInstigator->GetPawn();

	// 공격 범위 내에 있으면 공격 상태로 전환합니다.
	if (IsInsideAttackRadius()) {
		EnemyState = EEnemyState::EES_Attacking;
	}
	// 공격 범위 밖이면 타겟을 추격합니다.
	else if (IsOutsideAttackRadius()) {
		ChaseTarget();
	}

	return DamageAmount;
}

// 이 액터가 파괴될 때 호출되는 함수
void AEnemy::Destroyed()
{
	// 장착된 무기를 파괴합니다.
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	// 두 번째 무기도 있다면 파괴합니다.
	if (EquippedWeaponSecond) {
		EquippedWeaponSecond->Destroy();
	}
}


// 액터가 공격을 받았을 때의 구현입니다.
void AEnemy::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	// 부모 클래스의 GetHit_Implementation 함수를 호출합니다.
	Super::GetHit_Implementation(ImpactPoint, Hitter);

	// 적이 죽지 않았다면 체력 바를 표시합니다.
	if (!IsDead()) ShowHealthBar();

	// 순찰과 공격 타이머를 초기화합니다.
	ClearPatrolTimer();
	ClearAttackTimer();

	// 무기 충돌을 비활성화합니다.
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// 공격 모션을 중단합니다.
	StopAttackMontage();

	// 공격 범위 안에 있다면, 공격 타이머를 시작합니다.
	if (IsInsideAttackRadius()) {
		if (!IsDead()) {
			StartAttackTimer();
		}
	}
}

// 게임이 시작되거나 이 액터가 스폰될 때 호출됩니다.
void AEnemy::BeginPlay()
{
	// 부모 클래스의 BeginPlay 함수를 호출합니다.
	Super::BeginPlay();

	// PawnSensing 컴포넌트가 있다면 PawnSeen 함수를 바인딩합니다.
	if (PawnSensing) PawnSensing->OnSeePawn.AddDynamic(this, &AEnemy::PawnSeen);

	// 적을 초기화합니다.
	InitializeEnemy();

	// 이 액터에 "Enemy" 태그를 추가합니다.
	Tags.Add(FName("Enemy"));
}

// 적이 죽는 경우의 구현입니다.
void AEnemy::Die_Implementation()
{
	// 부모 클래스의 Die_Implementation 함수를 호출합니다.
	Super::Die_Implementation();

	// 적의 상태를 Dead로 설정합니다.
	EnemyState = EEnemyState::EES_Dead;

	// 공격 타이머와 체력 바를 숨깁니다.
	ClearAttackTimer();
	HideHealthBar();

	// 캡슐 컴포넌트를 비활성화합니다.
	DisableCapsule();

	// 이 액터의 수명을 설정합니다.
	SetLifeSpan(DeathLifeSpan);

	// 캐릭터의 이동 방향과 회전을 동기화하지 않도록 설정합니다.
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 무기의 충돌을 비활성화합니다.
	SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);

	// 소울을 생성합니다.
	SpawnSoul();
}

// 소울을 생성하는 함수입니다.
void AEnemy::SpawnSoul()
{
	// 소울 클래스와 속성이 있다면 소울을 스폰합니다.
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

// 적이 공격하는 함수입니다.
void AEnemy::Attack()
{
	// 부모 클래스의 Attack 함수를 호출합니다.
	Super::Attack();

	// 전투 대상이 없다면 함수를 빠져나옵니다.
	if (CombatTarget == nullptr) return;

	// 적의 상태를 Engaged로 설정합니다.
	EnemyState = EEnemyState::EES_Engaged;

	// 공격 모션을 실행합니다.
	PlayAttackMontage();
}

// 적이 공격할 수 있는지 확인하는 함수입니다.
bool AEnemy::CanAttack()
{
	// 공격 가능한 조건들을 검사합니다.
	bool bCanAttack =
		IsInsideAttackRadius() &&
		!IsAttacking() &&
		!IsEngaged() &&
		!IsDead();
	return bCanAttack;
}

// 적이 공격을 마친 후 호출되는 함수입니다.
void AEnemy::AttackEnd()
{
	// 적의 상태를 초기화합니다.
	EnemyState = EEnemyState::EES_NoState;

	// 전투 대상을 다시 확인합니다.
	CheckCombatTarget();
}

// 적이 데미지를 받았을 때의 처리를 합니다.
void AEnemy::HandleDamage(float DamageAmount)
{
	// 부모 클래스의 HandleDamage 함수를 호출합니다.
	Super::HandleDamage(DamageAmount);

	// 체력 바와 속성이 있다면 체력 바를 업데이트합니다.
	if (Attributes && HealthBarWidget)
	{
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
}



// 적을 초기화하는 함수입니다.
void AEnemy::InitializeEnemy()
{
	// 적의 컨트롤러를 AAIController로 캐스팅합니다.
	EnemyController = Cast<AAIController>(GetController());

	// 순찰 대상까지 이동을 시작합니다.
	MoveToTarget(PatrolTarget);

	// 체력 바를 숨깁니다.
	HideHealthBar();

	// 기본 무기를 스폰합니다.
	SpawnDefaultWeapon();
}

// 순찰 대상을 확인하는 함수입니다.
void AEnemy::CheckPatrolTarget()
{
	// 순찰 대상과의 거리가 PatrolRadius 이내인지 확인합니다.
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{
		// 새로운 순찰 대상을 선택합니다.
		PatrolTarget = ChoosePatrolTarget();

		// 순찰 대기 시간을 랜덤으로 설정합니다.
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);

		// 순찰이 끝난 후 호출할 함수와 대기 시간을 설정합니다.
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerFinished, WaitTime);
	}
}

// 전투 대상을 확인하는 함수입니다.
void AEnemy::CheckCombatTarget()
{
	// 전투 반경 밖에 있는지 확인합니다.
	if (IsOutsideCombatRadius())
	{
		// 공격 타이머를 초기화하고 관심을 잃습니다.
		ClearAttackTimer();
		LoseInterest();

		// 전투 상태가 아니면 순찰을 시작합니다.
		if (!IsEngaged()) StartPatrolling();
	}
	else if (IsOutsideAttackRadius() && !IsChasing())
	{
		// 공격 타이머를 초기화합니다.
		ClearAttackTimer();

		// 전투 상태가 아니면 대상을 추격합니다.
		if (!IsEngaged()) ChaseTarget();
	}
	else if (CanAttack())
	{
		// 공격 타이머를 시작합니다.
		StartAttackTimer();
	}
}

// 순찰 타이머가 끝났을 때 호출되는 함수입니다.
void AEnemy::PatrolTimerFinished()
{
	// 다시 순찰 대상까지 이동을 시작합니다.
	MoveToTarget(PatrolTarget);
}

// 체력 바를 숨기는 함수입니다.
void AEnemy::HideHealthBar()
{
	// HealthBarWidget이 존재하면 체력 바를 숨깁니다.
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

// 체력 바를 표시하는 함수입니다.
void AEnemy::ShowHealthBar()
{
	// HealthBarWidget이 존재하면 체력 바를 표시합니다.
	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

// 적이 전투 대상에 대한 관심을 잃는 함수입니다.
void AEnemy::LoseInterest()
{
	// 전투 대상을 null로 설정하고 체력 바를 숨깁니다.
	CombatTarget = nullptr;
	HideHealthBar();
}

// 순찰을 시작하는 함수입니다.
void AEnemy::StartPatrolling()
{
	// 적의 상태를 순찰로 설정합니다.
	EnemyState = EEnemyState::EES_Patrolling;

	// 캐릭터의 이동 속도를 순찰 속도로 설정합니다.
	GetCharacterMovement()->MaxWalkSpeed = PatrollingSpeed;

	// 순찰 대상까지 이동을 시작합니다.
	MoveToTarget(PatrolTarget);
}

// 전투 대상을 추격하는 함수입니다.
void AEnemy::ChaseTarget()
{
	// 적의 상태를 추격으로 설정합니다.
	EnemyState = EEnemyState::EES_Chasing;

	// 캐릭터의 이동 속도를 추격 속도로 설정합니다.
	GetCharacterMovement()->MaxWalkSpeed = ChasingSpeed;

	// 전투 대상까지 이동을 시작합니다.
	MoveToTarget(CombatTarget);

	// 추격 사운드가 설정되어 있다면 사운드를 재생합니다.
	if (ChaseSound) {
		UGameplayStatics::PlaySoundAtLocation(this, ChaseSound, this->GetActorLocation());
	}
}


// 전투 반경 밖에 있는지 확인하는 메서드
bool AEnemy::IsOutsideCombatRadius()
{
	return !InTargetRange(CombatTarget, CombatRadius);  // 전투 반경 내에 없으면 true 반환
}

// 공격 반경 밖에 있는지 확인하는 메서드
bool AEnemy::IsOutsideAttackRadius()
{
	return !InTargetRange(CombatTarget, AttackRadius);  // 공격 반경 내에 없으면 true 반환
}

// 공격 반경 안에 있는지 확인하는 메서드
bool AEnemy::IsInsideAttackRadius()
{
	return InTargetRange(CombatTarget, AttackRadius);  // 공격 반경 내에 있으면 true 반환
}

// 적이 추적 중인지 확인하는 메서드
bool AEnemy::IsChasing()
{
	return EnemyState == EEnemyState::EES_Chasing;  // 적의 상태가 '추적 중'이면 true 반환
}

// 적이 공격 중인지 확인하는 메서드
bool AEnemy::IsAttacking()
{
	return EnemyState == EEnemyState::EES_Attacking;  // 적의 상태가 '공격 중'이면 true 반환
}

// 적이 죽었는지 확인하는 메서드
bool AEnemy::IsDead()
{
	return EnemyState == EEnemyState::EES_Dead;  // 적의 상태가 '죽음'이면 true 반환
}

// 적이 전투 중인지 확인하는 메서드
bool AEnemy::IsEngaged()
{
	return EnemyState == EEnemyState::EES_Engaged;  // 적의 상태가 '전투 중'이면 true 반환
}

// 순찰 타이머를 초기화하는 메서드
void AEnemy::ClearPatrolTimer()
{
	GetWorldTimerManager().ClearTimer(PatrolTimer);  // 순찰 타이머를 초기화
}

// 공격 타이머를 시작하는 메서드
void AEnemy::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;  // 적의 상태를 '공격 중'으로 설정
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);  // 랜덤한 공격 시간을 설정
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);  // 공격 타이머를 설정
}

// 공격 타이머를 초기화하는 메서드
void AEnemy::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);  // 공격 타이머를 초기화
}

// 대상이 특정 반경 내에 있는지 확인하는 메서드. Target은 대상 객체, Radius는 반경
bool AEnemy::InTargetRange(AActor* Target, double Radius)
{
	if (Target == nullptr) return false;  // 대상이 없으면 false 반환
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();  // 대상과의 거리를 계산
	return DistanceToTarget <= Radius;  // 거리가 반경 내에 있으면 true 반환
}

// 대상까지 이동하는 메서드. Target은 이동할 대상
void AEnemy::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;  // 컨트롤러나 대상이 없으면 리턴
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);  // 이동할 대상 설정
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);  // 인식 반경 설정
	EnemyController->MoveTo(MoveRequest);  // 이동 명령 실행
}

// 순찰 대상을 선택하는 메서드
AActor* AEnemy::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)  // 모든 순찰 대상을 순회
	{
		if (Target != PatrolTarget)  // 현재 순찰 대상이 아니면
		{
			ValidTargets.AddUnique(Target);  // 유효한 대상 배열에 추가
		}
	}

	const int32 NumPatrolTargets = ValidTargets.Num();  // 유효한 순찰 대상의 수
	if (NumPatrolTargets > 0)  // 유효한 대상이 있으면
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);  // 랜덤한 대상을 선택
		return ValidTargets[TargetSelection];  // 선택한 대상을 반환
	}
	return nullptr;  // 유효한 대상이 없으면 nullptr 반환
}

// 기본 무기를 생성하는 메서드
void AEnemy::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();  // 현재 월드를 가져옴
	if (World && WeaponClass)  // 월드와 무기 클래스가 있으면
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);  // 무기를 생성
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket1"), this, this);  // 무기 장착
		EquippedWeapon = DefaultWeapon;  // 장착된 무기를 설정
	}

	// 두 번째 무기 슬롯이 있는 경우
	if (World && WeaponClass && GetMesh()->DoesSocketExist("WeaponSocket2")) {
		AWeapon* DefaultWeaponSecond = World->SpawnActor<AWeapon>(WeaponClass);  // 두 번째 무기를 생성
		DefaultWeaponSecond->Equip(GetMesh(), FName("WeaponSocket2"), this, this);  // 두 번째 무기 장착
		EquippedWeaponSecond = DefaultWeaponSecond;  // 장착된 두 번째 무기를 설정
	}
	else {
		EquippedWeaponSecond = nullptr;  // 두 번째 무기 슬롯이 없으면 nullptr 설정
	}
}

// 적을 볼 때 호출되는 메서드
void AEnemy::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&  // 죽지 않았고
		EnemyState != EEnemyState::EES_Chasing &&  // 이미 추적 중이지 않고
		EnemyState < EEnemyState::EES_Attacking&&  // 공격 중이 아니고
		SeenPawn->ActorHasTag(FName("EngageableTarget"));  // 본 적이 공격 가능한 대상이라면

	if (bShouldChaseTarget)  // 추적해야 한다면
	{
		CombatTarget = SeenPawn;  // 전투 대상을 설정
		ClearPatrolTimer();  // 순찰 타이머를 초기화
		ChaseTarget();  // 대상을 추적
	}
}
