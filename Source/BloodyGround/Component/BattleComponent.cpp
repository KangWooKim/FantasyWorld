#include "BattleComponent.h"
#include "Net/UnrealNetwork.h"

UBattleComponent::UBattleComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    bIsAiming = false;

    // ��Ʈ��ũ ���� Ȱ��ȭ
    SetIsReplicatedByDefault(true);

}

void UBattleComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UBattleComponent, bIsAiming);
}

void UBattleComponent::BeginPlay()
{
	Super::BeginPlay();

}


void UBattleComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UBattleComponent::StartAiming()
{
    bIsAiming = true;
}

void UBattleComponent::StopAiming()
{
    bIsAiming = false;
}

void UBattleComponent::StartFire()
{
}