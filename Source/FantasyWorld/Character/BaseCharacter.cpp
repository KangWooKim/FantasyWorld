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
	// �� Actor�� �� ƽ���� ������Ʈ�Ǿ�� �մϴ�.
	PrimaryActorTick.bCanEverTick = true;

	// ��Ʈ�ѷ��� ȸ���� ������� �ʽ��ϴ�.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// ĳ������ �̵� �������� ȸ���ϵ��� �����մϴ�.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// ĳ������ �޽��� �浹 ������ �����մϴ�.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// ī�޶�� ī�޶� ���� �����մϴ�.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	ActionState = EActionState::EAS_Unoccupied;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// �±׸� �����մϴ�.
	Tags.Add(FName("Character"));
	Tags.AddUnique(FName("EngageableTarget"));
	SpawnDefaultWeapon();
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	if (PlayerController) {
		HUD = Cast<ANormalLevelHUD>(PlayerController->GetHUD());
		if (HUD) {
			CombatOverlay = HUD->GetCombatOverlay();
		}
	}

	if (CombatOverlay) {
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}
}


// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	if (PlayerController) {
		HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
		if (HUD) {
			CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
		}
	}

	if (Attributes) {
		Attributes->RegenStamina(DeltaTime);
		if (CombatOverlay) {
			CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
		}
	}

	if (CurrentLockedTarget && bUseLockOnTarget) {
		FVector LookAtDirection = CurrentLockedTarget->GetActorLocation() - GetActorLocation();
		LookAtDirection.Z = 0; // ���� ���⸸ ���
		FRotator TargetRotation = LookAtDirection.Rotation();
		FRotator CurrentRotation = GetController()->GetControlRotation();

		// �ε巴�� ȸ��
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 3.f); // ������ �Ű������� ���� �ӵ��Դϴ�.

		GetController()->SetControlRotation(NewRotation);
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ĳ������ �̵��� ���� ��ȯ�� ���� �Լ��� �� �����մϴ�.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(FName("OpenMenu"), IE_Pressed, this, &ABaseCharacter::OpenMenu);
	PlayerInputComponent->BindAction(FName("LockOnTarget"), IE_Pressed, this, &ABaseCharacter::LockOnTarget);
}

void ABaseCharacter::LockOnTarget()
{
	if (bUseLockOnTarget == false) {
		bUseLockOnTarget = true;

		AActor* ClosestEnemy = GetClosestEnemy();
		if (ClosestEnemy) {
			CurrentLockedTarget = ClosestEnemy;
		}
	}
	else {
		bUseLockOnTarget = false;
		CurrentLockedTarget = nullptr; // ���� ���
	}
}

AActor* ABaseCharacter::GetClosestEnemy()
{
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies); // �� Ŭ���� ã��

	float ClosestDistanceSqr = FLT_MAX;
	AActor* ClosestEnemy = nullptr;

	for (AActor* Enemy : FoundEnemies) {
		float DistanceSqr = FVector::DistSquared(Enemy->GetActorLocation(), GetActorLocation());
		if (DistanceSqr < ClosestDistanceSqr) {
			ClosestDistanceSqr = DistanceSqr;
			ClosestEnemy = Enemy;
		}
	}

	return ClosestEnemy;
}

void ABaseCharacter::OpenMenu()
{
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (GetWorld() && PlayerController) {
		PlayerController->SetPause(true);
	}
	if (HUD) {
		HUD->ShowSettingsOverlay();
		if (PlayerController) {
			PlayerController->SetShowMouseCursor(true);
			PlayerController->UnPossess();
		}
	}
}

// �յڷ� �����̱� ���� �޼��� �Դϴ�.
void ABaseCharacter::MoveForward(float Value)
{

	// �׼� ���°� Unoccupied�� �ƴ϶�� �����մϴ�.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// ������ ������� Ȯ���մϴ�.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// �Է� �������� �̵��մϴ�.
		AddMovementInput(Direction, Value);

	}
}

// �¿�� �����̱� ���� �޼��� �Դϴ�.
void ABaseCharacter::MoveRight(float Value)
{
	// �׼� ���°� Unoccupied�� �ƴ϶�� �����մϴ�.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// �������� ������� Ȯ���մϴ�.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// �Է� �������� �̵��մϴ�.
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

// ���콺�� �̿��� ĳ������ ���� ��ȯ�� ���� �޼��� �Դϴ�.
void ABaseCharacter::Turn(float Value)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController) {
		AddControllerYawInput(Value * PlayerController->GetMouseSensitivity());
	}
	AddControllerYawInput(Value);
}

// ���콺�� �̿��� �� �Ʒ��� ���� ��ȯ�� ���� �޼��� �Դϴ�.
void ABaseCharacter::LookUp(float Value)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController) {
		AddControllerPitchInput(Value * PlayerController->GetMouseSensitivity());
	}
	AddControllerPitchInput(Value);
}

void ABaseCharacter::LethalMode() {
	if (!Attributes) return;
	if (LethalState == ELethalState::ELS_On) {
		LethalState = ELethalState::ELS_Off;
	}
	else if (LethalState == ELethalState::ELS_Off && Attributes->GetSouls() >= 50) {
		LethalState = ELethalState::ELS_On;
		if (LethalModeSound) {
			UGameplayStatics::PlaySoundAtLocation(this, LethalModeSound, this->GetActorLocation(), GameInstance->GetEffectVolume());
		}
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

void ABaseCharacter::LethalModeFinish() {
	LethalState = ELethalState::ELS_Off;
}

void ABaseCharacter::Attack() {
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
		if (Attributes) {
			Attributes->UseStamina(Attributes->GetAttackCost());
			if (CombatOverlay) {
				CombatOverlay->UpdateStaminaBar(Attributes->GetStaminaPercent());
			}
		}
		
	}
}

int32 ABaseCharacter::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 ABaseCharacter::PlayDeathMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage)
	{
		AnimInstance->Montage_Play(DeathMontage);
	}
	return int32();
}

void ABaseCharacter::StopAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance) {
		AnimInstance->Montage_Stop(0.25f, AttackMontage);
	}
}

int32 ABaseCharacter::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if (SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() - 1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}

AFantasyPlayerController* ABaseCharacter::GetPlayerController()
{
	return PlayerController;
}

void ABaseCharacter::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

bool ABaseCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_Unoccupied && Attributes && Attributes->GetStamina() >= Attributes->GetAttackCost();
}

bool ABaseCharacter::IsAlive()
{
	return Attributes && Attributes->IsAlive();
}

void ABaseCharacter::GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter)
{
	if (IsAlive() && Hitter)
	{
		DirectionalHitReact(Hitter->GetActorLocation());
		PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
		if (CombatCameraShake) {
			PlayerController->ClientStartCameraShake(CombatCameraShake);
		}
		PlayHitSound(ImpactPoint);
	}
	else Die();

	
	SpawnHitParticles(ImpactPoint);

	//SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ABaseCharacter::Die_Implementation()
{
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(this, DeathSound, this->GetActorLocation(), GameInstance->GetEffectVolume());
	}
	Tags.Add(FName("Dead"));
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayDeathMontage();
	ActionState = EActionState::EAS_Dead;
}

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

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

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

	PlayHitReactMontage(Section);
}

void ABaseCharacter::AttackEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

void ABaseCharacter::AddSouls(ASoul* Soul)
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD) {
		CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
	}
	if (Attributes && CombatOverlay) {
		Attributes->AddSouls(Soul->GetSouls());
		CombatOverlay->UpdateSoulText(Attributes->GetSouls());
	}
}

void ABaseCharacter::HitReactEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

void ABaseCharacter::DeathEnd() {
	
	OnDeathEnd.Broadcast();
	Destroy();
}

void ABaseCharacter::Destroy()
{
	Super::Destroy();
}

void ABaseCharacter::HandleDamage(float DamageAmount)
{
	if (Attributes)
	{
		Attributes->ReceiveDamage(DamageAmount);
	}
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HandleDamage(DamageAmount);
	SetHUDHealth();
	return DamageAmount;
}

void ABaseCharacter::SetHUDHealth()
{
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	HUD = HUD == nullptr ? Cast<ANormalLevelHUD>(PlayerController->GetHUD()) : HUD;
	if (HUD) {
		CombatOverlay = CombatOverlay == nullptr ? HUD->GetCombatOverlay() : CombatOverlay;
	}
	if (CombatOverlay && Attributes) {
		CombatOverlay->UpdateHealthBar(Attributes->GetHealthPercent());
	}
}

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

void ABaseCharacter::SpawnDefaultWeapon()
{
	UWorld* World = GetWorld();
	if (World && WeaponClass)
	{
		AWeapon* DefaultWeapon = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeapon->Equip(GetMesh(), FName("WeaponSocket1"), this, this);
		EquippedWeapon = DefaultWeapon;
	}

	if (World && WeaponClass && GetMesh()->DoesSocketExist("WeaponSocket2")) {
		AWeapon* DefaultWeaponSecond = World->SpawnActor<AWeapon>(WeaponClass);
		DefaultWeaponSecond->Equip(GetMesh(), FName("WeaponSocket2"), this, this);
		EquippedWeaponSecond = DefaultWeaponSecond;
	}
	else {
		EquippedWeaponSecond = EquippedWeapon;
	}
}