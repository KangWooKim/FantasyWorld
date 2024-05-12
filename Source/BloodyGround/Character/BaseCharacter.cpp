#include "BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "BloodyGround/Weapon/BaseWeapon.h"
#include "BloodyGround/Weapon/MachineGun.h"
#include "BloodyGround/Weapon/Pistol.h"
#include "BloodyGround/Component/InventoryComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BloodyGround/BloodyGroundGameModeBase.h"
#include "BloodyGround/HUD/InGameHUD.h"
#include "BloodyGround/HUD/InGameWidget.h"

// Ŭ���� ������
ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// ü�� �ʱ�ȭ
	Health = 100.0f;

	// ��Ʈ��ũ ������ Ȱ��ȭ
	SetReplicates(true);
	SetReplicateMovement(true);

	// BattleComponent �߰�
	BattleComp = CreateDefaultSubobject<UBattleComponent>(TEXT("BattleComponent"));

	// ī�޶� �� ���� �� ����
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // ī�޶�� ĳ���� ������ �Ÿ�
	CameraBoom->bUsePawnControlRotation = true; // ī�޶� ĳ������ ȸ���� ����
	CameraBoom->SocketOffset = FVector(0.0f, 50.0f, 50.0f); // ī�޶� ��ġ ����

	// ī�޶� ���� �� ����
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // ī�޶� �տ� ī�޶� ����
	FollowCamera->bUsePawnControlRotation = false; // ī�޶� ���� ȸ���� ������ �ʰ� ��

	// FOV �ʱⰪ ����
	DefaultFOV = 90.0f;
	AimedFOV = 65.0f;  // ���� �� FOV ��
	FOVInterpSpeed = 20.0f; // FOV ���� �ӵ�

	// �κ��丮 ������Ʈ �ʱ�ȭ
	InventoryComp = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.AddUnique(FName("Player"));

	CharacterState = ECharacterState::None;

	PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController) 
	{
		PlayerHUD = Cast<AInGameHUD>(PlayerController->GetHUD());
	}

	if (PlayerHUD)
	{
		PlayerHUD->UpdateHealth(1.f);
		PlayerHUD->DeleteRespawnText();
	}
	
	// �⺻ ���� �� ź�� ����
	if (HasAuthority())
	{
		// ���� ���� �� ����
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		APistol* NewPistol = GetWorld()->SpawnActor<APistol>(PistolBlueprint, this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
		if (NewPistol)
		{
			InventoryComp->AddWeapon(NewPistol);
			InventoryComp->SetInitWeapon(NewPistol);
			NewPistol->SetActorHiddenInGame(false);

			// ���⸦ ĳ������ ���Ͽ� ����
			const FName WeaponSocketName(TEXT("WeaponSocket_Pistol")); // ���� �̸�
			NewPistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);

		}

		// �ӽŰ� ���� �� ����
		AMachineGun* NewMachineGun = GetWorld()->SpawnActor<AMachineGun>(MachineGunBlueprint, this->GetActorLocation(), this->GetActorRotation(), SpawnParams);
		if (NewMachineGun)
		{
			InventoryComp->AddWeapon(NewMachineGun);
			NewMachineGun->SetActorHiddenInGame(true);

			// ���⸦ ĳ������ ���Ͽ� ����
			const FName WeaponSocketName(TEXT("WeaponSocket_Rifle")); // ���� �̸�
			NewMachineGun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);

		}

		// ź�� ����
		InventoryComp->SetPistolAmmo(50);
		InventoryComp->SetMachineGunAmmo(300);
	}

	// ī�޶� �⺻ FOV ����
	if (FollowCamera)
	{
		FollowCamera->SetFieldOfView(DefaultFOV);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// ���� ���¿� ���� ĳ���� ȸ�� ó��
	HandleAimingRotation();

	// FOV ���� ó��
	InterpFOV(DeltaTime);
}

void ABaseCharacter::HandleAimingRotation()
{
	if (BattleComp && BattleComp->bIsAiming && Controller)
	{
		// ���� ���� �� ��Ʈ�ѷ��� ȸ���� ����
		const FRotator NewRotation = Controller->GetControlRotation();
		SetActorRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
	}
	// ���� ���� �ƴ� ���� ������ ȸ�� ó���� ���� ���� (�⺻ ������ ���� ���)
}

void ABaseCharacter::HitReactionEnd()
{
	CharacterState = ECharacterState::None;
}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// ü�� ���� ���� ����
	DOREPLIFETIME(ABaseCharacter, Health);
	DOREPLIFETIME(ABaseCharacter, CharacterState);
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CharacterState == ECharacterState::Death || DamageCauser && DamageCauser->IsA(ABaseCharacter::StaticClass()))
	{
		return 0.f;
	}

	Health -= DamageAmount;

	if (PlayerHUD) {
		PlayerHUD->UpdateHealth(Health / 100.f);
	}

	if (Health <= 0)
	{
		HandleDeath();
		if (PlayerHUD)
		{
			PlayerHUD->SetRespawnText();
		}
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	
	CharacterState = ECharacterState::HitReact;
	
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseCharacter::HandleDeath()
{
	CharacterState = ECharacterState::Death;

	// ��� �Է� ��Ȱ��ȭ
	DisableInput(Cast<APlayerController>(GetController()));

	// ��� ó�� �� 5�� �ڿ� Respawn �Լ� ȣ��
	FTimerHandle RespawnTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, this, &ABaseCharacter::Respawn, 5.0f, false);
}

void ABaseCharacter::Respawn()
{
	// GameMode�� ���� �� ĳ���� ����
	if (ABloodyGroundGameModeBase* GM = Cast<ABloodyGroundGameModeBase>(GetWorld()->GetAuthGameMode()))
	{
		GM->RespawnPlayer(Cast<APlayerController>(GetController()));
	}

	InventoryComp->DestroyAllWeapons();
	// ����� ĳ���� ����
	Destroy();
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// �̵� �� ���� �Է� ���ε�
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &ABaseCharacter::LookUp);


	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::Reload);
	PlayerInputComponent->BindAction("ChangeWeapon", IE_Pressed, this, &ABaseCharacter::ChangeWeapon);
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::AttackButtonPressed);
	PlayerInputComponent->BindAction("Attack", IE_Released, this, &ABaseCharacter::AttackButtonReleased);
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &ABaseCharacter::AimButtonPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ABaseCharacter::AimButtonReleased);
}

void ABaseCharacter::Reload()
{
	InventoryComp->GetCurrentWeapon()->Reload();
}

void ABaseCharacter::ChangeWeapon()
{
	if (HasAuthority())
	{
		InventoryComp->ChangeWeapon();
	}
	else
	{
		ServerChangeWeapon();
	}
}

void ABaseCharacter::ServerChangeWeapon_Implementation()
{
	InventoryComp->ChangeWeapon();
}

bool ABaseCharacter::ServerChangeWeapon_Validate()
{
	return true; // ��ȿ�� �˻� ����
}

void ABaseCharacter::ServerFootStep_Implementation()
{
	this->MakeNoise(FootstepLoudness, this, GetActorLocation());
}

// ��/�� �̵� ó��
void ABaseCharacter::MoveForward(float Value)
{
	if (CharacterState != ECharacterState::None)
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// ĳ������ ���� ã��
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
		ServerFootStep();
	}
}

// ��/�� �̵� ó��
void ABaseCharacter::MoveRight(float Value)
{
	if (CharacterState != ECharacterState::None)
	{
		return;
	}

	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// ĳ������ ���� ã��
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
		ServerFootStep();
	}
}

void ABaseCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void ABaseCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void ABaseCharacter::Jump()
{
	if (CharacterState != ECharacterState::None)
	{
		return;
	}

	Super::Jump();
	ServerFootStep();
}

void ABaseCharacter::AttackButtonPressed()
{
	if (InventoryComp && InventoryComp->GetCurrentWeapon())
	{
		InventoryComp->GetCurrentWeapon()->Fire();
	}
}

void ABaseCharacter::AttackButtonReleased()
{
	if (HasAuthority())
	{
		if (InventoryComp && InventoryComp->GetCurrentWeapon())
		{
			InventoryComp->GetCurrentWeapon()->FireEnd();
		}
	}
	else
	{
		ServerStopAttack();
	}
	
}

void ABaseCharacter::ServerStopAttack_Implementation()
{
	MulticastStopAttack();
}

void ABaseCharacter::MulticastStopAttack_Implementation()
{
	if (InventoryComp && InventoryComp->GetCurrentWeapon())
	{
		InventoryComp->GetCurrentWeapon()->FireEnd();
	}
}

void ABaseCharacter::AimButtonPressed()
{
	if (BattleComp)
	{
		BattleComp->StartAiming();  // BattleComponent�� ���� ������ �˸�
		StartAiming();              // BaseCharacter ���ο��� �߰����� ���� ���� ó��
	}
}

void ABaseCharacter::AimButtonReleased()
{
	if (BattleComp)
	{
		BattleComp->StopAiming();   // BattleComponent�� ���� ���Ḧ �˸�
		StopAiming();               // BaseCharacter ���ο��� �߰����� ���� ���� ���� ó��
	}
}

void ABaseCharacter::StartAiming()
{
}

void ABaseCharacter::StopAiming()
{
}

void ABaseCharacter::InterpFOV(float DeltaTime)
{
	if (FollowCamera)
	{
		float TargetFOV = BattleComp && BattleComp->bIsAiming ? AimedFOV : DefaultFOV;
		float NewFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, FOVInterpSpeed);
		FollowCamera->SetFieldOfView(NewFOV);
	}
}