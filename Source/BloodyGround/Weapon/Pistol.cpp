#include "Pistol.h"
#include "BloodyGround/Component/InventoryComponent.h"
#include "Net/UnrealNetwork.h"
#include "BloodyGround/Character/BaseCharacter.h"
#include "BloodyGround/HUD/InGameHUD.h"

APistol::APistol()
{
    Capacity = 10;
    CurrentAmmo = Capacity;
    Damage = 10;

}

void APistol::Fire()
{
    if (CanFire()) return;
   
    Super::Fire();

    CurrentAmmo--;

    if (Character->GetInGameHUD())
    {
        Character->GetInGameHUD()->UpdateAmmo(CurrentAmmo, Character->InventoryComp->GetPistolAmmo());
    }
}

bool APistol::CanFire()
{
    return WeaponState == EWeaponState::Reload || Character == nullptr || Character->InventoryComp == nullptr || Character->InventoryComp->GetPistolAmmo() < 1 || CurrentAmmo < 1;
}

EWeaponType APistol::GetCurrentWeaponType()
{
    return EWeaponType::Pistol;
}

void APistol::PerformReload()
{
    Super::PerformReload();

    if (Character == nullptr || Character->InventoryComp == nullptr || WeaponState != EWeaponState::None)
    {
        return; // ĳ���� �Ǵ� �κ��丮 ������Ʈ�� ���� ��� ����
    }

    int32 AmmoToReload = FMath::Min(Capacity - CurrentAmmo, Character->InventoryComp->GetPistolAmmo());
    CurrentAmmo += AmmoToReload;
    Character->InventoryComp->SetMachineGunAmmo(Character->InventoryComp->GetPistolAmmo() - AmmoToReload);

    if (Character->GetInGameHUD())
    {
        Character->GetInGameHUD()->UpdateAmmo(CurrentAmmo, Character->InventoryComp->GetPistolAmmo());
    }
}

void APistol::ChangeWeapon()
{
    Super::ChangeWeapon();

    Character->GetInGameHUD()->UpdateAmmo(CurrentAmmo, Character->InventoryComp->GetPistolAmmo());
}
