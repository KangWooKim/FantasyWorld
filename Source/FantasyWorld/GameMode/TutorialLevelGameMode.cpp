// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

ATutorialLevelGameMode::ATutorialLevelGameMode()
{
}

void ATutorialLevelGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (BackGroundMusic) {
		UGameplayStatics::PlaySound2D(this, BackGroundMusic, 1.f, 1.f, 0.f, nullptr, nullptr, true);
	}
}
