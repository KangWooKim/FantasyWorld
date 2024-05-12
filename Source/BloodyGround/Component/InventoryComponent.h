#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BloodyGround/Weapon/BaseWeapon.h"
#include "BloodyGround/Weapon/Pistol.h"
#include "BloodyGround/Weapon/MachineGun.h"
#include "InventoryComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BLOODYGROUND_API UInventoryComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UInventoryComponent();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(Replicated)
        TArray<ABaseWeapon*> Weapons; // �÷��̾ ������ ������� ���

    UPROPERTY(Replicated)
        ABaseWeapon* CurrentWeapon; // ���� Ȱ��ȭ�� ����

    // ź�� �� ����
    UPROPERTY(Replicated)
        int32 PistolAmmo;

    UPROPERTY(Replicated)
        int32 MachineGunAmmo;

public:
    // ���� �߰� �� ����
    void AddWeapon(ABaseWeapon* NewWeapon);
    void RemoveWeapon(ABaseWeapon* WeaponToRemove);
    void SetInitWeapon(ABaseWeapon* Weapon);
    void DestroyAllWeapons();

    FORCEINLINE int32 GetPistolAmmo() { return PistolAmmo; }
    FORCEINLINE int32 GetMachineGunAmmo() { return MachineGunAmmo; }
    

    // ���� ��ȯ
    void ChangeWeapon();

    // ���� ���� ��������
    ABaseWeapon* GetCurrentWeapon() const;

    // ź�� ���� �Լ�
    void SetPistolAmmo(int32 NewAmmo);
    void SetMachineGunAmmo(int32 NewAmmo);


    // ��Ʈ��ũ ������ ���� �Լ�
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};