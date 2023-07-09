// Fill out your copyright notice in the Description page of Project Settings.

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterTypes.h"
#include "Engine/EngineTypes.h"
#include "BaseCharacter.h"

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

	// ī�޶�� ī�޶� ���� �����մϴ�.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 300.f;

	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
	ViewCamera->SetupAttachment(CameraBoom);


}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// �±׸� �����մϴ�.
	Tags.Add(FName("Character"));
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

	// ĳ������ �̵��� ���� ��ȯ�� ���� �Լ��� �� �����մϴ�.
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(FName("Turn"), this, &ABaseCharacter::Turn);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &ABaseCharacter::LookUp);

	PlayerInputComponent->BindAction(FName("Jump"), IE_Pressed, this, &ABaseCharacter::Jump);
	PlayerInputComponent->BindAction(FName("LethalMode"), IE_Pressed, this, &ABaseCharacter::LethalMode);
	PlayerInputComponent->BindAction(FName("Attack"), IE_Pressed, this, &ABaseCharacter::Attack);
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
	AddControllerYawInput(Value);
}

// ���콺�� �̿��� �� �Ʒ��� ���� ��ȯ�� ���� �޼��� �Դϴ�.
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

}