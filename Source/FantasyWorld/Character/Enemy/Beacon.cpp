// Fill out your copyright notice in the Description page of Project Settings.


#include "Beacon.h"
#include "FantasyWorld/Character/BaseCharacter.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/HUD/TutorialLevelHUD.h"
#include "FantasyWorld/HUD/TutorialLevelOverlay.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// 기본값을 설정하는 생성자입니다.
ABeacon::ABeacon()
{
	// 이 액터가 매 프레임마다 Tick() 함수를 호출하도록 설정합니다. 필요 없으면 끄면 성능 향상이 있을 수 있습니다.
	PrimaryActorTick.bCanEverTick = false;
	// 캡슐 컴포넌트를 생성하고 이름을 "Capsule"로 지정합니다.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	// OnOverlapBegin 함수를 OnActorBeginOverlap 이벤트에 연결합니다.
	OnActorBeginOverlap.AddDynamic(this, &ABeacon::OnOverlapBegin);
}

// 게임이 시작되거나 스폰될 때 호출됩니다.
void ABeacon::BeginPlay()
{
	Super::BeginPlay();

}

// 매 프레임마다 호출됩니다.
void ABeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// 액터가 다른 액터와 겹칠 때 호출되는 함수입니다.
void ABeacon::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// 겹친 액터가 존재하고 "Character" 태그를 가지고 있다면
	if (OtherActor && OtherActor->ActorHasTag(FName("Character"))) {
		// 첫 번째 플레이어 컨트롤러를 가져옵니다.
		AFantasyPlayerController* Controller = Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (Controller) {
			// HUD를 가져와서 튜토리얼 레벨 오버레이의 가시성을 활성화합니다.
			ATutorialLevelHUD* HUD = Cast<ATutorialLevelHUD>(Controller->GetHUD());
			if (HUD) {
				HUD->GetTutorialLevelOverlay()->VisibilityEnabled();
				// 캐릭터의 움직임을 즉시 중단시킵니다.
				ACharacter* Character = Cast<ACharacter>(Controller->GetPawn());
				if (Character && Character->GetCharacterMovement()) {
					Character->GetCharacterMovement()->StopMovementImmediately();
				}
				// 캐릭터를 언포즈 상태로 만듭니다.
				Controller->UnPossess();
			}
		}
	}
	// 이 Beacon 액터를 파괴합니다.
	Destroy();
}
