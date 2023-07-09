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

	// 카메라와 카메라 붐을 설정합니다.
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
	
	// 태그를 설정합니다.
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

}