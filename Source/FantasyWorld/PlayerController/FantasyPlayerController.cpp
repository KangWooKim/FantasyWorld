// Fill out your copyright notice in the Description page of Project Settings.

#include "FantasyPlayerController.h"
#include "Kismet/GameplayStatics.h"


void AFantasyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Character"), FoundActors);
	if (FoundActors.Num() > 0) {
		APawn* MyPawn = Cast<APawn>(FoundActors[0]);
		if (MyPawn) {
			Possess(MyPawn);
		}
	}
}