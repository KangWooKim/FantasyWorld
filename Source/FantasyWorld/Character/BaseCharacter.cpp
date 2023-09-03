// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterTypes.h"
#include "FantasyWorld/Component/AttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/Weapon/Weapon.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/CameraShake/CombatCameraShake.h"
#include "FantasyWorld/HUD/TutorialLevelHUD.h"
#include "FantasyWorld/HUD/TutorialLevelOverlay.h"
#include "FantasyWorld/HUD/CombatOverlay.h"
#include "FantasyWorld/HUD/NormalLevelHUD.h"
#include "FantasyWorld/Pickups/Soul.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"
#include "Enemy/Enemy.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// 이 Actor는 매 틱(tick)마다 업데이트 됩니다.
	// 틱은 게임에서의 한 프레임을 의미합니다.
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터가 회전할 때, 컨트롤러의 회전을 사용하지 않습니다.
	bUseControllerRotationPitch = false; // 상하 회전 미사용
	bUseControllerRotationYaw = false;   // 좌우 회전 미사용
	bUseControllerRotationRoll = false;  // 롤 회전 미사용

	// 캐릭터가 이동할 때, 이동 방향에 따라 자동으로 회전하도록 설정합니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);  // 회전 속도 설정

	// 캐릭터 메시의 충돌 설정입니다.
	// 여기서는 메시가 동적 오브젝트로 설정되고, 모든 채널에 대해 무시하되 특정 채널만 반응하도록 설정합니다.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);  // 오버랩 이벤트 활성화

	// 캡슐 컴포넌트의 카메라 채널에 대한 충돌을 무시합니다.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// 카메라와 카메라 붐(스프링 암) 설정
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;  // 카메라와의 거리 설정

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);  // 카메라를 카메라 붐에 붙임

	// 캐릭터의 속성을 관리하는 컴포넌트를 생성
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// 초기 액션 상태 설정 (EAS_Unoccupied는 캐릭터가 현재 특별한 액션을 하지 않고 있음을 의미)
	ActionState = EActionState::EAS_Unoccupied;
}

// 게임이 시작되거나 스폰될 때 호출됩니다.
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// 캐릭터에 태그를 추가합니다. 이는 나중에 캐릭터를 쉽게 식별할 수 있도록 도와줍니다.
	Tags.Add(FName("Character"));
	Tags.AddUnique(FName("EngageableTarget"));

	// 기본 무기를 스폰합니다.
	SpawnDefaultWeapon();

	// 컨트롤러와 게임 인스턴스를 얻습니다. nullptr이면 캐스팅을 시도합니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// HUD와 전투 오버레이를 설정합니다.
	if (PlayerController) {
		HUD = Cast<ANormalLevelHUD>(PlayerController->GetHUD());
		if (HUD) {
			CombatOverlay = HUD->GetCombatOverlay();
		}
	}

	// 전투 오버레이에 영혼 수를 업데이트합니다.
	if (CombatOverlay) {
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}
}

// 프레임마다 호출되는 Tick 함수입니다.
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// PlayerController와 GameInstance를 업데이트합니다. nullptr일 경우 캐스팅을 시도합니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// HUD와 전투 오버레이를 업데이트합니다. nullptr일 경우 캐스팅을 시도합니다.
	if (PlayerController) {
		HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD) {
			CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
		}
	}

	// 스태미나를 회복하고 전투 오버레이의 스태미나 바를 업데이트합니다.
	if (Attributes) {
		Attributes->RegenStamina(DeltaTime);
		if (CombatOverlay) {
			CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
		}
	}

	// 현재 잠금된 대상이 있고, 대상 잠금을 사용할 경우 캐릭터가 대상을 바라보도록 설정합니다.
	if (CurrentLockedTarget && bUseLockOnTarget) {
		FVector LookAtDirection = CurrentLockedTarget->GetActorLocation() - GetActorLocation();  // 대상과의 방향을 계산
		LookAtDirection.Z = 0;  // Z축은 무시 (수평만 고려)
		FRotator TargetRotation = LookAtDirection.Rotation();  // 바라볼 방향의 회전값을 계산
		FRotator CurrentRotation = GetController()->GetControlRotation();  // 현재 회전값을 가져옴

		// 부드럽게 대상을 바라보도록 회전값을 보간합니다.
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 3.f);
		GetController()->SetControlRotation(NewRotation);
	}
}

// 플레이어의 입력을 처리하는 함수입니다.
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동과 회전을 위한 축 입력을 바인딩합니다.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);  // 전후 이동
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);      // 좌우 이동
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);                // 좌우 회전
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);            // 상하 회전

	// 액션 입력을 바인딩합니다.
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ABaseCharacter::Jump);           // 점프
	PlayerInputComponent->BindAction(FName("OpenMenu"), IE_Pressed, this, &ABaseCharacter::OpenMenu);    // 메뉴 열기
	PlayerInputComponent->BindAction(FName("LockOnTarget"), IE_Pressed, this, &ABaseCharacter::LockOnTarget);  // 대상 잠금
}


// 대상 잠금 토글 기능을 처리하는 메서드입니다.
void ABaseCharacter::LockOnTarget()
{
	// 대상 잠금이 비활성화 상태라면 활성화합니다.
	if (bUseLockOnTarget == false) {
		bUseLockOnTarget = true;

		// 가장 가까운 적을 찾습니다.
		AActor* ClosestEnemy = GetClosestEnemy();
		// 가장 가까운 적이 있다면 현재 잠금 대상으로 설정합니다.
		if (ClosestEnemy) {
			CurrentLockedTarget = ClosestEnemy;
		}
	}
	else {
		// 이미 대상 잠금이 활성화 상태라면 비활성화하고 현재 잠금 대상을 nullptr로 설정합니다.
		bUseLockOnTarget = false;
		CurrentLockedTarget = nullptr; // 락온 취소
	}
}

// 가장 가까운 적을 찾는 메서드입니다.
AActor* ABaseCharacter::GetClosestEnemy()
{
	TArray<AActor*> FoundEnemies;
	// GetWorld()을 이용해 게임 세계에서 AEnemy 클래스를 모두 찾습니다.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

	float ClosestDistanceSqr = FLT_MAX;
	AActor* ClosestEnemy = nullptr;

	// 찾은 적들 중 가장 가까운 적을 찾습니다.
	for (AActor* Enemy : FoundEnemies) {
		float DistanceSqr = FVector::DistSquared(Enemy->GetActorLocation(), GetActorLocation());
		if (DistanceSqr < ClosestDistanceSqr) {
			ClosestDistanceSqr = DistanceSqr;
			ClosestEnemy = Enemy;
		}
	}

	return ClosestEnemy;
}

// 메뉴를 여는 메서드입니다.
void ABaseCharacter::OpenMenu()
{
	// HUD를 업데이트합니다. nullptr인 경우 캐스팅을 시도합니다.
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

	// 게임을 일시 중지합니다.
	if (GetWorld() && PlayerController) {
		PlayerController->SetPause(true);
	}

	// 설정 오버레이를 보여줍니다.
	if (HUD) {
		HUD->ShowSettingsOverlay();
		// 마우스 커서를 보여주고, 캐릭터 조종을 해제합니다.
		if (PlayerController) {
			PlayerController->SetShowMouseCursor(true);
			PlayerController->UnPossess();
		}
	}
}

// 앞으로 이동하는 메서드입니다. Value는 이동 속도(방향)를 나타냅니다.
void ABaseCharacter::MoveForward(float Value)
{
	// 액션 상태가 Unoccupied가 아니라면 함수를 빠져나갑니다.
	if (ActionState != EActionState::EAS_Unoccupied) return;

	// 컨트롤러가 존재하고, 이동 값(Value)가 0이 아니라면 실행합니다.
	if (Controller && (Value != 0.f))
	{
		// 캐릭터의 현재 회전 값을 가져와 Yaw만을 고려한 회전 값을 계산합니다.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// 이동할 방향을 계산합니다.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 캐릭터를 해당 방향으로 이동시킵니다.
		AddMovementInput(Direction, Value);
	}
}

// 오른쪽으로 이동하는 메서드입니다. Value는 이동 속도(방향)를 나타냅니다.
void ABaseCharacter::MoveRight(float Value)
{
	// 액션 상태가 Unoccupied가 아니라면 함수를 빠져나갑니다.
	if (ActionState != EActionState::EAS_Unoccupied) return;

	// 컨트롤러가 존재하고, 이동 값(Value)가 0이 아니라면 실행합니다.
	if (Controller && (Value != 0.f))
	{
		// 캐릭터의 현재 회전 값을 가져와 Yaw만을 고려한 회전 값을 계산합니다.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// 이동할 방향을 계산합니다.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 캐릭터를 해당 방향으로 이동시킵니다.
		AddMovementInput(Direction, Value);
	}
}


// 캐릭터의 점프를 위한 메서드 입니다.
void ABaseCharacter::StartJump()
{
	bPressedJump = true;
}

// 캐릭터의 점프 종료를 위한 메서드입니다.
void ABaseCharacter::StopJump()
{
	bPressedJump = false;
}

// 마우스를 이용한 캐릭터의 방향 전환을 위한 메서드입니다.
void ABaseCharacter::Turn(float Value)
{
	// 플레이어 컨트롤러를 가져옵니다. 없을 경우 현재 캐릭터의 컨트롤러를 사용합니다.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController) {
		// 마우스 감도를 곱해서 컨트롤러의 Yaw 입력값을 추가합니다.
		AddControllerYawInput(Value * PlayerController->GetMouseSensitivity());
	}
	AddControllerYawInput(Value); // Yaw 입력값 추가
}

// 마우스를 이용해 위 아래로 방향 전환을 위한 메서드입니다.
void ABaseCharacter::LookUp(float Value)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController) {
		// 마우스 감도를 곱해서 컨트롤러의 Pitch 입력값을 추가합니다.
		AddControllerPitchInput(Value * PlayerController->GetMouseSensitivity());
	}
	AddControllerPitchInput(Value); // Pitch 입력값 추가
}

// 필살기 모드를 활성화하는 메서드입니다.
void ABaseCharacter::LethalMode() {
	if (!Attributes) return; // 속성 정보가 없을 경우 종료
	if (LethalState == ELethalState::ELS_On) {
		LethalState = ELethalState::ELS_Off; // 필살기 모드가 활성화되어 있을 경우 비활성화
	}
	else if (LethalState == ELethalState::ELS_Off && Attributes->GetSouls() >= 50) {
		// 필살기 모드가 비활성화되어 있고 소울 수가 충분할 경우 활성화
		LethalState = ELethalState::ELS_On;

		// 필살기 모드 사운드 재생
		if (LethalModeSound) {
			UGameplayStatics::PlaySoundAtLocation(this, LethalModeSound, this->GetActorLocation(), GameInstance->GetEffectVolume());
		}

		// 필살기 모드 파티클 생성
		if (LethalModeParticle && GetWorld())
		{
			UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				LethalModeParticle,
				this->GetActorLocation()
			);
		}
	}
}

// 필살기 모드를 비활성화하는 메서드입니다.
void ABaseCharacter::LethalModeFinish() {
	LethalState = ELethalState::ELS_Off;
}

// 공격을 수행하는 메서드입니다.
void ABaseCharacter::Attack() {
	if (CanAttack())
	{
		PlayAttackMontage(); // 공격 몽타주 재생
		ActionState = EActionState::EAS_Attacking;

		// 스태미나 소모 및 스태미나 바 업데이트
		if (Attributes) {
			Attributes->UseStamina(Attributes->GetAttackCost());
			if (CombatOverlay) {
				CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
			}
		}
	}
}

// 무작위 공격 몽타주 섹션을 재생하는 메서드입니다.
int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

// 사망 몽타주를 재생하는 메서드입니다.
int32 ABaseCharacter::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage); // 사망 몽타주 재생
	}
	return int32(); // 결과 반환
}


// 공격 몽타주를 중지하는 메서드입니다.
void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		// 공격 몽타주를 일정 시간 내에 중지합니다.
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

// 무작위로 몽타주 섹션을 재생하는 메서드입니다.
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	// 선택된 섹션을 재생합니다.
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

// 플레이어 컨트롤러를 반환하는 메서드입니다.
AFantasyPlayerController* ABaseCharacter::GetPlayerController()
{
	return PlayerController;
}

// 몽타주 섹션을 재생하는 메서드입니다.
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		// 몽타주를 재생하고 특정 섹션으로 이동합니다.
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

// 공격 가능 여부를 확인하는 메서드입니다.
bool ABaseCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && Attributes && Attributes->GetStamina() >= Attributes->GetAttackCost();
}

// 생존 여부를 확인하는 메서드입니다.
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

// 피격 시 호출되는 메서드입니다. (가상 함수 구현)
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		// 피격 방향에 따른 히트 리액션을 수행합니다.
		DirectionalHitReact(Hitter->GetActorLocation());
		PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
		if (CombatCameraShake) {
			// 전투 카메라 쉐이크를 화면에 적용합니다.
			PlayerController->ClientStartCameraShake(CombatCameraShake);
		}
		PlayHitSound(ImpactPoint);
	}
	else Die();

	// 피격 파티클을 생성합니다.
	SpawnHitParticles(ImpactPoint);

	// 캐릭터의 체력이 0 이상인 경우 히트 리액션 상태로 전환합니다.
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}


// 캐릭터 사망 시 호출되는 메서드입니다. (가상 함수 구현)
void ABaseCharacter::Die_Implementation()
{
	// 사망 사운드를 재생합니다.
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation(), GameInstance->GetEffectVolume());
	}

	// "Dead" 태그를 추가하여 사망 상태를 표시합니다.
	Tags.Add(FName("Dead"));

	// 캐릭터 메시의 충돌을 해제합니다.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 사망 몽타주를 재생합니다.
	PlayDeathMontage();

	// 액션 상태를 사망으로 설정합니다.
	ActionState = EActionState::EAS_Dead;
}

// 피격 사운드를 재생하는 메서드입니다.
void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint,
			GameInstance->GetEffectVolume()
		);
	}
}

// 피격 파티클을 생성하는 메서드입니다.
void ABaseCharacter::SpawnHitParticles(const FVector& ImpactPoint)
{
	if (HitParticles && GetWorld())
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticles,
			ImpactPoint
		);
	}
}

// 피격 리액션 몽타주를 재생하는 메서드입니다.
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		// 피격 리액션 몽타주를 재생하고 특정 섹션으로 이동합니다.
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


// 특정 방향에 맞은 경우 피격 리액션을 재생하는 메서드입니다.
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// 피격 지점의 Z 좌표를 현재 캐릭터의 높이로 낮춥니다.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1 이므로 Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// 역 코사인을 취하여 theta를 얻습니다.
	double Theta = FMath::Acos(CosTheta);
	// 라디안 값을 도 단위로 변환합니다.
	Theta = FMath::RadiansToDegrees(Theta);

	// CrossProduct의 Z 값이 음수인 경우, Theta를 음수로 만듭니다.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	// Theta 각도에 따라 피격 리액션 섹션을 결정합니다.
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

	PlayHitReactMontage(Section); // 결정된 섹션으로 피격 리액션 몽타주 재생
}

// 공격 종료 시 호출되는 메서드입니다.
void ABaseCharacter::AttackEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

// 소울을 추가하는 메서드입니다.
void ABaseCharacter::AddSouls(ASoul* Soul)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD) {
		CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
	}
	if (Attributes && CombatOverlay) {
		// 소울을 추가하고 화면에 업데이트합니다.
		Attributes->AddSouls(Soul->GetSouls());
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}
}

// 피격 리액션 종료 시 호출되는 메서드입니다.
void ABaseCharacter::HitReactEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

// 사망 애니메이션 종료 시 호출되는 메서드입니다.
void ABaseCharacter::DeathEnd() {
	OnDeathEnd.Broadcast();
	Destroy();
}

// 캐릭터 파괴 시 호출되는 메서드입니다.
void ABaseCharacter::Destroy()
{
	Super::Destroy(); // 기본 파괴 함수 호출
}


// 데미지 처리를 수행하는 메서드입니다.
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount); // Attributes의 Health에 반영합니다.
	}
}

// 데미지를 받을 때 호출되는 함수입니다.
float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount); // 데미지 처리 메서드 호출
	SetHUDHealth(); // HUD의 체력을 업데이트합니다.
	return DamageAmount; // 받은 데미지 값을 반환합니다.
}

// HUD의 체력을 설정하는 메서드입니다.
void ABaseCharacter::SetHUDHealth()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD) {
		CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
	}
	if (CombatOverlay && Attributes) {
		// 콤바트 오버레이를 이용해 체력바를 업데이트합니다.
		CombatOverlay->UpdateHealthBar(Attributes->GetHealthPercent());
	}
}

// 무기의 충돌을 설정하는 메서드입니다.
void ABaseCharacter::SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled)
{
	if (EquippedWeapon && EquippedWeapon->GetWeaponBox())
	{
		EquippedWeapon->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeapon->IgnoreActors.Empty();
	}

	if (EquippedWeaponSecond && EquippedWeaponSecond->GetWeaponBox())
	{
		EquippedWeaponSecond->GetWeaponBox()->SetCollisionEnabled(CollisionEnabled);
		EquippedWeaponSecond->IgnoreActors.Empty();
	}
}


// 기본 무기를 생성하고 장착하는 메서드입니다.
void ABaseCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		// 기본 무기를 생성하고 장착합니다.
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket1"), this, this);
		EquippedWeapon = DefaultWeapon;
	}

	if (World && WeaponClass && GetMesh()->DoesSocketExist("WeaponSocket2")) {
		// 두 번째 소켓이 있는 경우 두 번째 무기를 생성하고 장착합니다.
		AWeapon* DefaultWeaponSecond = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeaponSecond->Equip(GetMesh(), FName("WeaponSocket2"), this, this);
		EquippedWeaponSecond = DefaultWeaponSecond;
	}
	else {
		// 두 번째 소켓이 없는 경우 두 번째 무기를 첫 번째 무기와 동일하게 설정합니다.
		EquippedWeaponSecond = EquippedWeapon;
	}
}
