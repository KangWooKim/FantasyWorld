// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// �� ������Ʈ�� ���� ���۽� �ʱ�ȭ�ǰ�, �� �����Ӹ��� Tick�� ȣ��˴ϴ�.
	// �ʿ� ���ٸ� �� ����� �� �� �־� ���� ����� �����մϴ�.
	PrimaryComponentTick.bCanEverTick = false;

	// ... (���⿡�� �߰� �ʱ�ȭ �ڵ尡 �� �� �ֽ��ϴ�.)
}


// ������ ������ �� ȣ��Ǵ� �޼����Դϴ�.
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ... (���� ���۽� ������ ������ ���⿡ �߰��մϴ�.)
}


// �� �����Ӹ��� ȣ��Ǵ� �޼����Դϴ�.
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ... (�� �����Ӹ��� ������ ������ ���⿡ �߰��մϴ�.)
}

// �������� �޴� �Լ��Դϴ�.
void UAttributeComponent::ReceiveDamage(float Damage)
{
	// �������� �޾� Health�� ���̵�, 0�� MaxHealth ������ ������ �����մϴ�.
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

// ���׹̳��� ����ϴ� �Լ��Դϴ�.
void UAttributeComponent::UseStamina(float StaminaCost)
{
	// ���׹̳��� ����ϵ�, 0�� MaxStamina ������ ������ �����մϴ�.
	Stamina = FMath::Clamp(Stamina - StaminaCost, 0.f, MaxStamina);
}

// ���� ü���� �ۼ�Ƽ���� ��ȯ�ϴ� �Լ��Դϴ�.
float UAttributeComponent::GetHealthPercent()
{
	// ���� ü���� �ִ� ü������ ���� ���� ��ȯ�մϴ�.
	return Health / MaxHealth;
}

// ���� ���׹̳��� �ۼ�Ƽ���� ��ȯ�ϴ� �Լ��Դϴ�.
float UAttributeComponent::GetStaminaPercent()
{
	// ���� ���׹̳��� �ִ� ���׹̳��� ���� ���� ��ȯ�մϴ�.
	return Stamina / MaxStamina;
}

// ĳ���Ͱ� ����ִ��� Ȯ���ϴ� �Լ��Դϴ�.
bool UAttributeComponent::IsAlive()
{
	// ü���� 0 �ʰ��� ����ִٰ� �Ǵ��մϴ�.
	return Health > 0.f;
}

// �ҿ��� �߰��ϴ� �Լ��Դϴ�.
void UAttributeComponent::AddSouls(int32 NumberOfSouls)
{
	// �ҿ��� �߰��մϴ�.
	Souls += NumberOfSouls;
}

// ���� �߰��ϴ� �Լ��Դϴ�.
void UAttributeComponent::AddGold(int AmountOfGold)
{
	// ���� �߰��մϴ�.
	Gold += AmountOfGold;
}

// ���׹̳��� ȸ���ϴ� �Լ��Դϴ�.
void UAttributeComponent::RegenStamina(float DeltaTime)
{
	// DeltaTime�� ���׹̳� ȸ������ ���� ���� ���׹̳��� ���ϵ�, 0�� MaxStamina ������ ������ �����մϴ�.
	Stamina = FMath::Clamp(Stamina + StaminaRegenRate * DeltaTime, 0.f, MaxStamina);
}
