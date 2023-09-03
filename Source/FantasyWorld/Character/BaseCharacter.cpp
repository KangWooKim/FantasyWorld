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
	// �� Actor�� �� ƽ(tick)���� ������Ʈ �˴ϴ�.
	// ƽ�� ���ӿ����� �� �������� �ǹ��մϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// ĳ���Ͱ� ȸ���� ��, ��Ʈ�ѷ��� ȸ���� ������� �ʽ��ϴ�.
	bUseControllerRotationPitch = false; // ���� ȸ�� �̻��
	bUseControllerRotationYaw = false;   // �¿� ȸ�� �̻��
	bUseControllerRotationRoll = false;  // �� ȸ�� �̻��

	// ĳ���Ͱ� �̵��� ��, �̵� ���⿡ ���� �ڵ����� ȸ���ϵ��� �����մϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);  // ȸ�� �ӵ� ����

	// ĳ���� �޽��� �浹 �����Դϴ�.
	// ���⼭�� �޽ð� ���� ������Ʈ�� �����ǰ�, ��� ä�ο� ���� �����ϵ� Ư�� ä�θ� �����ϵ��� �����մϴ�.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);  // ������ �̺�Ʈ Ȱ��ȭ

	// ĸ�� ������Ʈ�� ī�޶� ä�ο� ���� �浹�� �����մϴ�.
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// ī�޶�� ī�޶� ��(������ ��) ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;  // ī�޶���� �Ÿ� ����

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);  // ī�޶� ī�޶� �տ� ����

	// ĳ������ �Ӽ��� �����ϴ� ������Ʈ�� ����
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	// �ʱ� �׼� ���� ���� (EAS_Unoccupied�� ĳ���Ͱ� ���� Ư���� �׼��� ���� �ʰ� ������ �ǹ�)
	ActionState = EActionState::EAS_Unoccupied;
}

// ������ ���۵ǰų� ������ �� ȣ��˴ϴ�.
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	// ĳ���Ϳ� �±׸� �߰��մϴ�. �̴� ���߿� ĳ���͸� ���� �ĺ��� �� �ֵ��� �����ݴϴ�.
	Tags.Add(FName("Character"));
	Tags.AddUnique(FName("EngageableTarget"));

	// �⺻ ���⸦ �����մϴ�.
	SpawnDefaultWeapon();

	// ��Ʈ�ѷ��� ���� �ν��Ͻ��� ����ϴ�. nullptr�̸� ĳ������ �õ��մϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// HUD�� ���� �������̸� �����մϴ�.
	if (PlayerController) {
		HUD = Cast<ANormalLevelHUD>(PlayerController->GetHUD());
		if (HUD) {
			CombatOverlay = HUD->GetCombatOverlay();
		}
	}

	// ���� �������̿� ��ȥ ���� ������Ʈ�մϴ�.
	if (CombatOverlay) {
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}
}

// �����Ӹ��� ȣ��Ǵ� Tick �Լ��Դϴ�.
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// PlayerController�� GameInstance�� ������Ʈ�մϴ�. nullptr�� ��� ĳ������ �õ��մϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// HUD�� ���� �������̸� ������Ʈ�մϴ�. nullptr�� ��� ĳ������ �õ��մϴ�.
	if (PlayerController) {
		HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD) {
			CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
		}
	}

	// ���¹̳��� ȸ���ϰ� ���� ���������� ���¹̳� �ٸ� ������Ʈ�մϴ�.
	if (Attributes) {
		Attributes->RegenStamina(DeltaTime);
		if (CombatOverlay) {
			CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
		}
	}

	// ���� ��ݵ� ����� �ְ�, ��� ����� ����� ��� ĳ���Ͱ� ����� �ٶ󺸵��� �����մϴ�.
	if (CurrentLockedTarget && bUseLockOnTarget) {
		FVector LookAtDirection = CurrentLockedTarget->GetActorLocation() - GetActorLocation();  // ������ ������ ���
		LookAtDirection.Z = 0;  // Z���� ���� (���� ���)
		FRotator TargetRotation = LookAtDirection.Rotation();  // �ٶ� ������ ȸ������ ���
		FRotator CurrentRotation = GetController()->GetControlRotation();  // ���� ȸ������ ������

		// �ε巴�� ����� �ٶ󺸵��� ȸ������ �����մϴ�.
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 3.f);
		GetController()->SetControlRotation(NewRotation);
	}
}

// �÷��̾��� �Է��� ó���ϴ� �Լ��Դϴ�.
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �̵��� ȸ���� ���� �� �Է��� ���ε��մϴ�.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);  // ���� �̵�
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);      // �¿� �̵�
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);                // �¿� ȸ��
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);            // ���� ȸ��

	// �׼� �Է��� ���ε��մϴ�.
	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ABaseCharacter::Jump);           // ����
	PlayerInputComponent->BindAction(FName("OpenMenu"), IE_Pressed, this, &ABaseCharacter::OpenMenu);    // �޴� ����
	PlayerInputComponent->BindAction(FName("LockOnTarget"), IE_Pressed, this, &ABaseCharacter::LockOnTarget);  // ��� ���
}


// ��� ��� ��� ����� ó���ϴ� �޼����Դϴ�.
void ABaseCharacter::LockOnTarget()
{
	// ��� ����� ��Ȱ��ȭ ���¶�� Ȱ��ȭ�մϴ�.
	if (bUseLockOnTarget == false) {
		bUseLockOnTarget = true;

		// ���� ����� ���� ã���ϴ�.
		AActor* ClosestEnemy = GetClosestEnemy();
		// ���� ����� ���� �ִٸ� ���� ��� ������� �����մϴ�.
		if (ClosestEnemy) {
			CurrentLockedTarget = ClosestEnemy;
		}
	}
	else {
		// �̹� ��� ����� Ȱ��ȭ ���¶�� ��Ȱ��ȭ�ϰ� ���� ��� ����� nullptr�� �����մϴ�.
		bUseLockOnTarget = false;
		CurrentLockedTarget = nullptr; // ���� ���
	}
}

// ���� ����� ���� ã�� �޼����Դϴ�.
AActor* ABaseCharacter::GetClosestEnemy()
{
	TArray<AActor*> FoundEnemies;
	// GetWorld()�� �̿��� ���� ���迡�� AEnemy Ŭ������ ��� ã���ϴ�.
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

	float ClosestDistanceSqr = FLT_MAX;
	AActor* ClosestEnemy = nullptr;

	// ã�� ���� �� ���� ����� ���� ã���ϴ�.
	for (AActor* Enemy : FoundEnemies) {
		float DistanceSqr = FVector::DistSquared(Enemy->GetActorLocation(), GetActorLocation());
		if (DistanceSqr < ClosestDistanceSqr) {
			ClosestDistanceSqr = DistanceSqr;
			ClosestEnemy = Enemy;
		}
	}

	return ClosestEnemy;
}

// �޴��� ���� �޼����Դϴ�.
void ABaseCharacter::OpenMenu()
{
	// HUD�� ������Ʈ�մϴ�. nullptr�� ��� ĳ������ �õ��մϴ�.
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;

	// ������ �Ͻ� �����մϴ�.
	if (GetWorld() && PlayerController) {
		PlayerController->SetPause(true);
	}

	// ���� �������̸� �����ݴϴ�.
	if (HUD) {
		HUD->ShowSettingsOverlay();
		// ���콺 Ŀ���� �����ְ�, ĳ���� ������ �����մϴ�.
		if (PlayerController) {
			PlayerController->SetShowMouseCursor(true);
			PlayerController->UnPossess();
		}
	}
}

// ������ �̵��ϴ� �޼����Դϴ�. Value�� �̵� �ӵ�(����)�� ��Ÿ���ϴ�.
void ABaseCharacter::MoveForward(float Value)
{
	// �׼� ���°� Unoccupied�� �ƴ϶�� �Լ��� ���������ϴ�.
	if (ActionState != EActionState::EAS_Unoccupied) return;

	// ��Ʈ�ѷ��� �����ϰ�, �̵� ��(Value)�� 0�� �ƴ϶�� �����մϴ�.
	if (Controller && (Value != 0.f))
	{
		// ĳ������ ���� ȸ�� ���� ������ Yaw���� ����� ȸ�� ���� ����մϴ�.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// �̵��� ������ ����մϴ�.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// ĳ���͸� �ش� �������� �̵���ŵ�ϴ�.
		AddMovementInput(Direction, Value);
	}
}

// ���������� �̵��ϴ� �޼����Դϴ�. Value�� �̵� �ӵ�(����)�� ��Ÿ���ϴ�.
void ABaseCharacter::MoveRight(float Value)
{
	// �׼� ���°� Unoccupied�� �ƴ϶�� �Լ��� ���������ϴ�.
	if (ActionState != EActionState::EAS_Unoccupied) return;

	// ��Ʈ�ѷ��� �����ϰ�, �̵� ��(Value)�� 0�� �ƴ϶�� �����մϴ�.
	if (Controller && (Value != 0.f))
	{
		// ĳ������ ���� ȸ�� ���� ������ Yaw���� ����� ȸ�� ���� ����մϴ�.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		// �̵��� ������ ����մϴ�.
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// ĳ���͸� �ش� �������� �̵���ŵ�ϴ�.
		AddMovementInput(Direction, Value);
	}
}


// ĳ������ ������ ���� �޼��� �Դϴ�.
void ABaseCharacter::StartJump()
{
	bPressedJump = true;
}

// ĳ������ ���� ���Ḧ ���� �޼����Դϴ�.
void ABaseCharacter::StopJump()
{
	bPressedJump = false;
}

// ���콺�� �̿��� ĳ������ ���� ��ȯ�� ���� �޼����Դϴ�.
void ABaseCharacter::Turn(float Value)
{
	// �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�. ���� ��� ���� ĳ������ ��Ʈ�ѷ��� ����մϴ�.
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController) {
		// ���콺 ������ ���ؼ� ��Ʈ�ѷ��� Yaw �Է°��� �߰��մϴ�.
		AddControllerYawInput(Value * PlayerController->GetMouseSensitivity());
	}
	AddControllerYawInput(Value); // Yaw �Է°� �߰�
}

// ���콺�� �̿��� �� �Ʒ��� ���� ��ȯ�� ���� �޼����Դϴ�.
void ABaseCharacter::LookUp(float Value)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController) {
		// ���콺 ������ ���ؼ� ��Ʈ�ѷ��� Pitch �Է°��� �߰��մϴ�.
		AddControllerPitchInput(Value * PlayerController->GetMouseSensitivity());
	}
	AddControllerPitchInput(Value); // Pitch �Է°� �߰�
}

// �ʻ�� ��带 Ȱ��ȭ�ϴ� �޼����Դϴ�.
void ABaseCharacter::LethalMode() {
	if (!Attributes) return; // �Ӽ� ������ ���� ��� ����
	if (LethalState == ELethalState::ELS_On) {
		LethalState = ELethalState::ELS_Off; // �ʻ�� ��尡 Ȱ��ȭ�Ǿ� ���� ��� ��Ȱ��ȭ
	}
	else if (LethalState == ELethalState::ELS_Off && Attributes->GetSouls() >= 50) {
		// �ʻ�� ��尡 ��Ȱ��ȭ�Ǿ� �ְ� �ҿ� ���� ����� ��� Ȱ��ȭ
		LethalState = ELethalState::ELS_On;

		// �ʻ�� ��� ���� ���
		if (LethalModeSound) {
			UGameplayStatics::PlaySoundAtLocation(this, LethalModeSound, this->GetActorLocation(), GameInstance->GetEffectVolume());
		}

		// �ʻ�� ��� ��ƼŬ ����
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

// �ʻ�� ��带 ��Ȱ��ȭ�ϴ� �޼����Դϴ�.
void ABaseCharacter::LethalModeFinish() {
	LethalState = ELethalState::ELS_Off;
}

// ������ �����ϴ� �޼����Դϴ�.
void ABaseCharacter::Attack() {
	if (CanAttack())
	{
		PlayAttackMontage(); // ���� ��Ÿ�� ���
		ActionState = EActionState::EAS_Attacking;

		// ���¹̳� �Ҹ� �� ���¹̳� �� ������Ʈ
		if (Attributes) {
			Attributes->UseStamina(Attributes->GetAttackCost());
			if (CombatOverlay) {
				CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
			}
		}
	}
}

// ������ ���� ��Ÿ�� ������ ����ϴ� �޼����Դϴ�.
int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

// ��� ��Ÿ�ָ� ����ϴ� �޼����Դϴ�.
int32 ABaseCharacter::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage); // ��� ��Ÿ�� ���
	}
	return int32(); // ��� ��ȯ
}


// ���� ��Ÿ�ָ� �����ϴ� �޼����Դϴ�.
void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		// ���� ��Ÿ�ָ� ���� �ð� ���� �����մϴ�.
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

// �������� ��Ÿ�� ������ ����ϴ� �޼����Դϴ�.
int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	// ���õ� ������ ����մϴ�.
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

// �÷��̾� ��Ʈ�ѷ��� ��ȯ�ϴ� �޼����Դϴ�.
AFantasyPlayerController* ABaseCharacter::GetPlayerController()
{
	return PlayerController;
}

// ��Ÿ�� ������ ����ϴ� �޼����Դϴ�.
void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		// ��Ÿ�ָ� ����ϰ� Ư�� �������� �̵��մϴ�.
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

// ���� ���� ���θ� Ȯ���ϴ� �޼����Դϴ�.
bool ABaseCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && Attributes && Attributes->GetStamina() >= Attributes->GetAttackCost();
}

// ���� ���θ� Ȯ���ϴ� �޼����Դϴ�.
bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

// �ǰ� �� ȣ��Ǵ� �޼����Դϴ�. (���� �Լ� ����)
void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		// �ǰ� ���⿡ ���� ��Ʈ ���׼��� �����մϴ�.
		DirectionalHitReact(Hitter->GetActorLocation());
		PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
		if (CombatCameraShake) {
			// ���� ī�޶� ����ũ�� ȭ�鿡 �����մϴ�.
			PlayerController->ClientStartCameraShake(CombatCameraShake);
		}
		PlayHitSound(ImpactPoint);
	}
	else Die();

	// �ǰ� ��ƼŬ�� �����մϴ�.
	SpawnHitParticles(ImpactPoint);

	// ĳ������ ü���� 0 �̻��� ��� ��Ʈ ���׼� ���·� ��ȯ�մϴ�.
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}


// ĳ���� ��� �� ȣ��Ǵ� �޼����Դϴ�. (���� �Լ� ����)
void ABaseCharacter::Die_Implementation()
{
	// ��� ���带 ����մϴ�.
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation(), GameInstance->GetEffectVolume());
	}

	// "Dead" �±׸� �߰��Ͽ� ��� ���¸� ǥ���մϴ�.
	Tags.Add(FName("Dead"));

	// ĳ���� �޽��� �浹�� �����մϴ�.
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// ��� ��Ÿ�ָ� ����մϴ�.
	PlayDeathMontage();

	// �׼� ���¸� ������� �����մϴ�.
	ActionState = EActionState::EAS_Dead;
}

// �ǰ� ���带 ����ϴ� �޼����Դϴ�.
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

// �ǰ� ��ƼŬ�� �����ϴ� �޼����Դϴ�.
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

// �ǰ� ���׼� ��Ÿ�ָ� ����ϴ� �޼����Դϴ�.
void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		// �ǰ� ���׼� ��Ÿ�ָ� ����ϰ� Ư�� �������� �̵��մϴ�.
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}


// Ư�� ���⿡ ���� ��� �ǰ� ���׼��� ����ϴ� �޼����Դϴ�.
void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// �ǰ� ������ Z ��ǥ�� ���� ĳ������ ���̷� ����ϴ�.
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1 �̹Ƿ� Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// �� �ڻ����� ���Ͽ� theta�� ����ϴ�.
	double Theta = FMath::Acos(CosTheta);
	// ���� ���� �� ������ ��ȯ�մϴ�.
	Theta = FMath::RadiansToDegrees(Theta);

	// CrossProduct�� Z ���� ������ ���, Theta�� ������ ����ϴ�.
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	// Theta ������ ���� �ǰ� ���׼� ������ �����մϴ�.
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

	PlayHitReactMontage(Section); // ������ �������� �ǰ� ���׼� ��Ÿ�� ���
}

// ���� ���� �� ȣ��Ǵ� �޼����Դϴ�.
void ABaseCharacter::AttackEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

// �ҿ��� �߰��ϴ� �޼����Դϴ�.
void ABaseCharacter::AddSouls(ASoul* Soul)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD) {
		CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
	}
	if (Attributes && CombatOverlay) {
		// �ҿ��� �߰��ϰ� ȭ�鿡 ������Ʈ�մϴ�.
		Attributes->AddSouls(Soul->GetSouls());
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}
}

// �ǰ� ���׼� ���� �� ȣ��Ǵ� �޼����Դϴ�.
void ABaseCharacter::HitReactEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

// ��� �ִϸ��̼� ���� �� ȣ��Ǵ� �޼����Դϴ�.
void ABaseCharacter::DeathEnd() {
	OnDeathEnd.Broadcast();
	Destroy();
}

// ĳ���� �ı� �� ȣ��Ǵ� �޼����Դϴ�.
void ABaseCharacter::Destroy()
{
	Super::Destroy(); // �⺻ �ı� �Լ� ȣ��
}


// ������ ó���� �����ϴ� �޼����Դϴ�.
void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount); // Attributes�� Health�� �ݿ��մϴ�.
	}
}

// �������� ���� �� ȣ��Ǵ� �Լ��Դϴ�.
float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount); // ������ ó�� �޼��� ȣ��
	SetHUDHealth(); // HUD�� ü���� ������Ʈ�մϴ�.
	return DamageAmount; // ���� ������ ���� ��ȯ�մϴ�.
}

// HUD�� ü���� �����ϴ� �޼����Դϴ�.
void ABaseCharacter::SetHUDHealth()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD) {
		CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
	}
	if (CombatOverlay && Attributes) {
		// �޹�Ʈ �������̸� �̿��� ü�¹ٸ� ������Ʈ�մϴ�.
		CombatOverlay->UpdateHealthBar(Attributes->GetHealthPercent());
	}
}

// ������ �浹�� �����ϴ� �޼����Դϴ�.
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


// �⺻ ���⸦ �����ϰ� �����ϴ� �޼����Դϴ�.
void ABaseCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		// �⺻ ���⸦ �����ϰ� �����մϴ�.
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket1"), this, this);
		EquippedWeapon = DefaultWeapon;
	}

	if (World && WeaponClass && GetMesh()->DoesSocketExist("WeaponSocket2")) {
		// �� ��° ������ �ִ� ��� �� ��° ���⸦ �����ϰ� �����մϴ�.
		AWeapon* DefaultWeaponSecond = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeaponSecond->Equip(GetMesh(), FName("WeaponSocket2"), this, this);
		EquippedWeaponSecond = DefaultWeaponSecond;
	}
	else {
		// �� ��° ������ ���� ��� �� ��° ���⸦ ù ��° ����� �����ϰ� �����մϴ�.
		EquippedWeaponSecond = EquippedWeapon;
	}
}
