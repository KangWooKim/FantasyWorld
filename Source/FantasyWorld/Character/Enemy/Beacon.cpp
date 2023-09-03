// Fill out your copyright notice in the Description page of Project Settings.


#include "Beacon.h"
#include "FantasyWorld/Character/BaseCharacter.h"
#include "FantasyWorld/PlayerController/FantasyPlayerController.h"
#include "FantasyWorld/HUD/TutorialLevelHUD.h"
#include "FantasyWorld/HUD/TutorialLevelOverlay.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// �⺻���� �����ϴ� �������Դϴ�.
ABeacon::ABeacon()
{
	// �� ���Ͱ� �� �����Ӹ��� Tick() �Լ��� ȣ���ϵ��� �����մϴ�. �ʿ� ������ ���� ���� ����� ���� �� �ֽ��ϴ�.
	PrimaryActorTick.bCanEverTick = false;
	// ĸ�� ������Ʈ�� �����ϰ� �̸��� "Capsule"�� �����մϴ�.
	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	// OnOverlapBegin �Լ��� OnActorBeginOverlap �̺�Ʈ�� �����մϴ�.
	OnActorBeginOverlap.AddDynamic(this, &ABeacon::OnOverlapBegin);
}

// ������ ���۵ǰų� ������ �� ȣ��˴ϴ�.
void ABeacon::BeginPlay()
{
	Super::BeginPlay();

}

// �� �����Ӹ��� ȣ��˴ϴ�.
void ABeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// ���Ͱ� �ٸ� ���Ϳ� ��ĥ �� ȣ��Ǵ� �Լ��Դϴ�.
void ABeacon::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// ��ģ ���Ͱ� �����ϰ� "Character" �±׸� ������ �ִٸ�
	if (OtherActor && OtherActor->ActorHasTag(FName("Character"))) {
		// ù ��° �÷��̾� ��Ʈ�ѷ��� �����ɴϴ�.
		AFantasyPlayerController* Controller = Cast<AFantasyPlayerController>(GetWorld()->GetFirstPlayerController());
		if (Controller) {
			// HUD�� �����ͼ� Ʃ�丮�� ���� ���������� ���ü��� Ȱ��ȭ�մϴ�.
			ATutorialLevelHUD* HUD = Cast<ATutorialLevelHUD>(Controller->GetHUD());
			if (HUD) {
				HUD->GetTutorialLevelOverlay()->VisibilityEnabled();
				// ĳ������ �������� ��� �ߴܽ�ŵ�ϴ�.
				ACharacter* Character = Cast<ACharacter>(Controller->GetPawn());
				if (Character && Character->GetCharacterMovement()) {
					Character->GetCharacterMovement()->StopMovementImmediately();
				}
				// ĳ���͸� ������ ���·� ����ϴ�.
				Controller->UnPossess();
			}
		}
	}
	// �� Beacon ���͸� �ı��մϴ�.
	Destroy();
}
