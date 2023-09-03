// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// 이 컴포넌트는 게임 시작시 초기화되고, 매 프레임마다 Tick이 호출됩니다.
	// 필요 없다면 이 기능을 끌 수 있어 성능 향상이 가능합니다.
	PrimaryComponentTick.bCanEverTick = false;

	// ... (여기에는 추가 초기화 코드가 올 수 있습니다.)
}


// 게임이 시작할 때 호출되는 메서드입니다.
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ... (게임 시작시 실행할 로직을 여기에 추가합니다.)
}


// 매 프레임마다 호출되는 메서드입니다.
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... (매 프레임마다 실행할 로직을 여기에 추가합니다.)
}

// 데미지를 받는 함수입니다.
void UAttributeComponent::ReceiveDamage(float Damage)
{
	// 데미지를 받아 Health를 줄이되, 0과 MaxHealth 사이의 값으로 유지합니다.
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

// 스테미나를 사용하는 함수입니다.
void UAttributeComponent::UseStamina(float StaminaCost)
{
	// 스테미나를 사용하되, 0과 MaxStamina 사이의 값으로 유지합니다.
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

// 현재 체력의 퍼센티지를 반환하는 함수입니다.
float UAttributeComponent::GetHealthPercent()
{
	// 현재 체력을 최대 체력으로 나눈 값을 반환합니다.
	return Health / MaxHealth;
}

// 현재 스테미나의 퍼센티지를 반환하는 함수입니다.
float UAttributeComponent::GetStaminaPercent()
{
	// 현재 스테미나를 최대 스테미나로 나눈 값을 반환합니다.
	return Stamina / MaxStamina;
}

// 캐릭터가 살아있는지 확인하는 함수입니다.
bool UAttributeComponent::IsAlive()
{
	// 체력이 0 초과면 살아있다고 판단합니다.
	return Health > 0.f;
}

// 소울을 추가하는 함수입니다.
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	// 소울을 추가합니다.
	Souls += NumberOfSouls;
}

// 금을 추가하는 함수입니다.
void UAttributeComponent::AddGold(int AmountOfGold)
{
	// 금을 추가합니다.
	Gold += AmountOfGold;
}

// 스테미나를 회복하는 함수입니다.
void UAttributeComponent::RegenStamina(float DeltaTime)
{
	// DeltaTime에 스테미나 회복률을 곱한 값을 스테미나에 더하되, 0과 MaxStamina 사이의 값으로 유지합니다.
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}
