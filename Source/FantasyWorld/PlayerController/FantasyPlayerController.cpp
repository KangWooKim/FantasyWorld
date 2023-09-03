// Fill out your copyright notice in the Description page of Project Settings.

#include "FantasyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"


// BeginPlay �Լ��� ���� ���� �� ȣ��Ǵ� �Լ��Դϴ�.
void AFantasyPlayerController::BeginPlay()
{
	Super::BeginPlay();  // �θ� Ŭ������ BeginPlay �Լ��� ȣ���մϴ�.

	// "Character" �±׸� ���� ��� ���͸� ã���ϴ�.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);

	// ã�� ���� �� ù ��° ���͸� MyPawn���� �����ϰ�, �̸� �����մϴ�.
	if (FoundActors.Num() > 0) {
		MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
			bShowMouseCursor = false;  // ���콺 Ŀ���� ����ϴ�.
		}
	}

	// GameInstance�� null�̶��, ���� ���� �ν��Ͻ��� �����ͼ� �����մϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
}

// PlayerTick �Լ��� �� �����Ӹ��� ȣ��Ǵ� �Լ��Դϴ�.
void AFantasyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);  // �θ� Ŭ������ PlayerTick �Լ��� ȣ���մϴ�.

	// GameInstance�� null�̶��, ���� ���� �ν��Ͻ��� �����ͼ� �����մϴ�.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance���� ���콺 �ΰ����� �����ɴϴ�.
	MouseSensitivity = GameInstance ? GameInstance->GetMouseSensitivity() : MouseSensitivity;

	float DeltaX, DeltaY;
	GetInputMouseDelta(DeltaX, DeltaY);  // ���콺�� �������� ����ϴ�.

	// ���콺 �ΰ����� �����մϴ�.
	DeltaX *= MouseSensitivity;
	DeltaY *= MouseSensitivity;

	// �̵� �Է¿� ���콺 �ΰ����� �����մϴ�.
	AddYawInput(DeltaX);
	AddPitchInput(DeltaY);
}

// RePossess �Լ��� ĳ���͸� �ٽ� �����ϱ� ���� ȣ��˴ϴ�.
void AFantasyPlayerController::RePossess()
{
	// "Character" �±׸� ���� ��� ���͸� ã���ϴ�.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);

	// ã�� ���� �� ù ��° ���͸� MyPawn���� �����ϰ�, �̸� �����մϴ�.
	if (FoundActors.Num() > 0) {
		MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
			bShowMouseCursor = false;  // ���콺 Ŀ���� ����ϴ�.
		}
	}
}

// GetPawn �Լ��� ���� ���� ���� ĳ����(Pawn)�� ��ȯ�մϴ�.
APawn* AFantasyPlayerController::GetPawn()
{
	return MyPawn;
}

// SetMouseSensitivity �Լ��� ���콺 �ΰ����� �����մϴ�.
// @parameter
// float Sensitivity : ������ ���콺 �ΰ��� ��
void AFantasyPlayerController::SetMouseSensitivity(float Sensitivity)
{
	MouseSensitivity = Sensitivity;
}
