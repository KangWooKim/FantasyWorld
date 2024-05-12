#include "InventoryComponent.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
    SetIsReplicatedByDefault(true);
}

void UInventoryComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UInventoryComponent::AddWeapon(ABaseWeapon* NewWeapon)
{
    if (NewWeapon && !Weapons.Contains(NewWeapon))
    {
        Weapons.Add(NewWeapon);
    }
}

void UInventoryComponent::RemoveWeapon(ABaseWeapon* WeaponToRemove)
{
    if (WeaponToRemove && Weapons.Contains(WeaponToRemove))
    {
        Weapons.Remove(WeaponToRemove);
    }
}

void UInventoryComponent::SetInitWeapon(ABaseWeapon* Weapon)
{
    CurrentWeapon = Weapon;
}

void UInventoryComponent::DestroyAllWeapons()
{
    for (int32 index = 0; index < Weapons.Num(); index++)
    {
        Weapons[index]->Destroy();
    }
}

void UInventoryComponent::ChangeWeapon()
{
    if (Weapons.Num() <= 1)
    {
        // ���Ⱑ �� �� ������ ��� ��ȯ�� �� ����
        return;
    }

    // ���� ���� �ε��� ã��
    int32 CurrentWeaponIndex = Weapons.IndexOfByKey(CurrentWeapon);
    int32 NextWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num(); // ���� ���� �ε��� ���

    ABaseWeapon* NewWeapon = Weapons[NextWeaponIndex];

    if (CurrentWeapon)
    {
        CurrentWeapon->WeaponState = EWeaponState::None;
        CurrentWeapon->SetActorHiddenInGame(true); // ���� ���� �����
    }

    CurrentWeapon = NewWeapon; // ���� ���� ������Ʈ
    CurrentWeapon->SetActorHiddenInGame(false); // �� ���� ���̱�
    CurrentWeapon->ChangeWeapon();

}

ABaseWeapon* UInventoryComponent::GetCurrentWeapon() const
{
    return CurrentWeapon;
}

void UInventoryComponent::SetPistolAmmo(int32 NewAmmo)
{
    PistolAmmo = NewAmmo;
}

void UInventoryComponent::SetMachineGunAmmo(int32 NewAmmo)
{
    MachineGunAmmo = NewAmmo;
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(UInventoryComponent, Weapons);
    DOREPLIFETIME(UInventoryComponent, CurrentWeapon);
    DOREPLIFETIME(UInventoryComponent, PistolAmmo);
    DOREPLIFETIME(UInventoryComponent, MachineGunAmmo);
}