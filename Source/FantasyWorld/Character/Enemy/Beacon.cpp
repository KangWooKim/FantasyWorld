// Fill out your copyright notice in the Description page of Project Settings.


#include "Beacon.h"
#include "FantasyWorld/Character/BaseCharacter.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/HUD/TutorialLevelHUD.h"
#include "FantasyWorld/HUD/TutorialLevelOverlay.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ABeacon::ABeacon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	OnActorBeginOverlap.AddDynamic(this, &ABeacon::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ABeacon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABeacon::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && OtherActor->ActorHasTag(FName("Character"))) {
		AFantasyPlayerController* Controller = Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (Controller) {
			ATutorialLevelHUD* HUD = Cast<ATutorialLevelHUD>(Controller->GetHUD());
			if (HUD) {
				HUD->GetTutorialLevelOverlay()->VisibilityEnabled();
				ACharacter* Character = Cast<ACharacter>(Controller->GetPawn());
				if (Character && Character->GetCharacterMovement()) {
					Character->GetCharacterMovement()->StopMovementImmediately();
				}
				Controller->UnPossess();
			}
		}
	}
	Destroy();
}

