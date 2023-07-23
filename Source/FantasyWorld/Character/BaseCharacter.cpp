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

// Sets default values
ABaseCharacter::ABaseCharacter()
{
	// 이 Actor는 매 틱마다 업데이트되어야 합니다.
	PrimaryActorTick.bCanEverTick = true;

	// 컨트롤러의 회전을 사용하지 않습니다.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터의 이동 방향으로 회전하도록 설정합니다.
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);

	// 캐릭터의 메시의 충돌 설정을 설정합니다.
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	// 카메라와 카메라 붐을 설정합니다.
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
	
	// 태그를 설정합니다.
	Tags.Add(FName("Character"));
	Tags.AddUnique(FName("EngageableTarget"));
	SpawnDefaultWeapon();
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	
}


// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 캐릭터의 이동과 방향 전환을 위한 함수를 축 매핑합니다.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &ABaseCharacter::LethalMode);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ABaseCharacter::Attack);
}

// 앞뒤로 움직이기 위한 메서드 입니다.
void ABaseCharacter::MoveForward(float Value)
{

	// 액션 상태가 Unoccupied가 아니라면 리턴합니다.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// 전방이 어디인지 확인합니다.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		// 입력 방향으로 이동합니다.
		AddMovementInput(Direction, Value);

	}
}

// 좌우로 움직이기 위한 메서드 입니다.
void ABaseCharacter::MoveRight(float Value)
{
	// 액션 상태가 Unoccupied가 아니라면 리턴합니다.
	if (ActionState != EActionState::EAS_Unoccupied) return;
	if (Controller && (Value != 0.f))
	{
		// 오른쪽이 어디인지 확인합니다.
		const FRotator ControlRotation = GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// 입력 방향으로 이동합니다.
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

// 마우스를 이용한 캐릭터의 방향 전환을 위한 메서드 입니다.
void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

// 마우스를 이용해 위 아래로 방향 전환을 위한 메서드 입니다.
void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABaseCharacter::LethalMode() {
	LethalState = ELethalState::ELS_On;
}

void ABaseCharacter::LethalModeFinish() {
	LethalState = ELethalState::ELS_Off;
}

void ABaseCharacter::Attack() {
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
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
	return ActionState == EActionState::EAS_Unoccupied;
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
		
	}
	else Die();

	PlayHitSound(ImpactPoint);
	SpawnHitParticles(ImpactPoint);

	//SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Attributes && Attributes->GetHealthPercent() > 0.f) {
		ActionState = EActionState::EAS_HitReaction;
	}
}

void ABaseCharacter::Die_Implementation()
{
	Tags.Add(FName("Dead"));
	PlayDeathMontage();
}

void ABaseCharacter::PlayHitSound(const FVector& ImpactPoint)
{
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			this,
			HitSound,
			ImpactPoint
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

void ABaseCharacter::HitReactEnd() {
	ActionState = EActionState::EAS_Unoccupied;
}

void ABaseCharacter::DeathEnd() {
	
	ActionState = EActionState::EAS_Dead;
	PlayerController = PlayerController == nullptr ? Cast<AFantasyPlayerController>(GetController()) : PlayerController;
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->UnPossess();
	}
	Destroy();
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
		EquippedWeaponSecond = nullptr;
	}
}