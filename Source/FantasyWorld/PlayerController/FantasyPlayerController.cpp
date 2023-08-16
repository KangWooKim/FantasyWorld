// Fill out your copyright notice in the Description page of Project Settings.

#include "FantasyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "FantasyWorld/GameInstance/MyGameInstance.h"


void AFantasyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);
	if (FoundActors.Num() > 0) {
		MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
			bShowMouseCursor = false;
		}
	}
	
	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
}

void AFantasyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	GameInstance = GameInstance == nullptr ? Cast<UMyGameInstance>(GetGameInstance()) : GameInstance;
	MouseSensitivity = GameInstance ? GameInstance->GetMouseSensitivity() : MouseSensitivity;
	

	float DeltaX, DeltaY;
	GetInputMouseDelta(DeltaX, DeltaY);

	// 민감도를 적용
	DeltaX *= MouseSensitivity;
	DeltaY *= MouseSensitivity;

	// 이동에 적용
	AddYawInput(DeltaX);
	AddPitchInput(DeltaY);

}

void AFantasyPlayerController::RePossess()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);
	if (FoundActors.Num() > 0) {
		MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
			bShowMouseCursor = false;
		}
	}
}

APawn* AFantasyPlayerController::GetPawn()
{
	return MyPawn;
}

void AFantasyPlayerController::SetMouseSensitivity(float Sensitivity)
{
	MouseSensitivity = Sensitivity;
}