// Fill out your copyright notice in the Description page of Project Settings.

#include "FantasyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"


// BeginPlay 함수는 게임 시작 시 호출되는 함수입니다.
void AFantasyPlayerController::BeginPlay()
{
	Super::BeginPlay();  // 부모 클래스의 BeginPlay 함수를 호출합니다.

	// "Character" 태그를 가진 모든 액터를 찾습니다.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);

	// 찾은 액터 중 첫 번째 액터를 MyPawn으로 설정하고, 이를 조종합니다.
	if (FoundActors.Num() > 0) {
		MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
			bShowMouseCursor = false;  // 마우스 커서를 숨깁니다.
		}
	}

	// GameInstance가 null이라면, 현재 게임 인스턴스를 가져와서 설정합니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
}

// PlayerTick 함수는 매 프레임마다 호출되는 함수입니다.
void AFantasyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);  // 부모 클래스의 PlayerTick 함수를 호출합니다.

	// GameInstance가 null이라면, 현재 게임 인스턴스를 가져와서 설정합니다.
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;

	// GameInstance에서 마우스 민감도를 가져옵니다.
	MouseSensitivity = GameInstance ? GameInstance->GetMouseSensitivity() : MouseSensitivity;

	float DeltaX, DeltaY;
	GetInputMouseDelta(DeltaX, DeltaY);  // 마우스의 움직임을 얻습니다.

	// 마우스 민감도를 적용합니다.
	DeltaX *= MouseSensitivity;
	DeltaY *= MouseSensitivity;

	// 이동 입력에 마우스 민감도를 적용합니다.
	AddYawInput(DeltaX);
	AddPitchInput(DeltaY);
}

// RePossess 함수는 캐릭터를 다시 조종하기 위해 호출됩니다.
void AFantasyPlayerController::RePossess()
{
	// "Character" 태그를 가진 모든 액터를 찾습니다.
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);

	// 찾은 액터 중 첫 번째 액터를 MyPawn으로 설정하고, 이를 조종합니다.
	if (FoundActors.Num() > 0) {
		MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
			bShowMouseCursor = false;  // 마우스 커서를 숨깁니다.
		}
	}
}

// GetPawn 함수는 현재 조종 중인 캐릭터(Pawn)을 반환합니다.
APawn* AFantasyPlayerController::GetPawn()
{
	return MyPawn;
}

// SetMouseSensitivity 함수는 마우스 민감도를 설정합니다.
// @parameter
// float Sensitivity : 설정할 마우스 민감도 값
void AFantasyPlayerController::SetMouseSensitivity(float Sensitivity)
{
	MouseSensitivity = Sensitivity;
}
