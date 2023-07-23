// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "FantasyWorld/Character/Enemy/Enemy.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnLocation = GetActorLocation();
	SpawnRotator = GetActorRotation().GetEquivalentRotator();
	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy()
{
	int32 SectionNumber = GetRandomEnemy();
	TSubclassOf<AEnemy> SpawnEnemy = EnemyList[SectionNumber];

	if (GetWorld()) {
		GetWorld()->SpawnActor<AEnemy>(SpawnEnemy, SpawnLocation, SpawnRotator);
	}
}

int32 AEnemySpawner::GetRandomEnemy()
{
	const int32 MaxSectionNumber = EnemyList.Num() - 1;
	const int32 SectionNumber = FMath::FMath::RandRange(0, MaxSectionNumber);
	return SectionNumber;
}

