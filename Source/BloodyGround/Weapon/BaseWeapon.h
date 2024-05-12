#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Animation/AnimMontage.h"
#include "BaseWeapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None    UMETA(DisplayName = "None"),
	Pistol  UMETA(DisplayName = "Pistol"),
	MachineGun UMETA(DisplayName = "MachineGun")
};

UENUM(BlueprintType)
enum class EWeaponState : uint8
{
    None    UMETA(DisplayName = "None"),
    Fire  UMETA(DisplayName = "Fire"),
    Reload  UMETA(DisplayName = "Reload")
};

UCLASS()
class BLOODYGROUND_API ABaseWeapon : public APawn
{
    GENERATED_BODY()

public:
    ABaseWeapon();

    UFUNCTION(NetMulticast, Reliable)
    // Ŭ���̾�Ʈ���� �߻� �ִϸ��̼��� ����ϱ� ���� �Լ�
    void MulticastPlayFireAnimation();

    UFUNCTION(Server, Reliable)
    void ServerPlayFireAnimation();

protected:
    virtual void BeginPlay() override;

    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
    USkeletalMeshComponent* SkeletalMeshComponent;

    UPROPERTY()
    class ABaseCharacter* Character;

    UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = Noise)
    class UPawnNoiseEmitterComponent* NoiseEmitter;

     virtual bool CanFire();

public:

    UFUNCTION()
    virtual void ChangeWeapon();

    virtual void Tick(float DeltaTime) override;

    // ���� �߻� �Լ�
    virtual void Fire();

    // źâ ���ε� �Լ�
    UFUNCTION(BlueprintCallable)
    void Reload();

    UFUNCTION()
     virtual void PerformReload();

    // �������� �������� ó���ϱ� ���� �Լ�
    UFUNCTION(Server, Reliable)
     void ServerReload();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastPlayReloadAnimation();

    virtual EWeaponType GetCurrentWeaponType();

    // źâ �� ���� ź�� ��
    UPROPERTY(Replicated ,EditDefaultsOnly, Category = "Weapon")
    int32 CurrentAmmo;

    // źâ �ִ� �뷮
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 Capacity;

    // ������ ���� ����
    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    int32 Damage;

    UFUNCTION(BlueprintCallable)
     virtual void FireEnd();

    UFUNCTION(BlueprintCallable)
     void ReloadEnd();

    FORCEINLINE USkeletalMeshComponent* GetWeaponMesh() {return SkeletalMeshComponent;}
    FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

    UPROPERTY(Replicated)
    EWeaponState WeaponState;

    // �߻� �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    class UAnimMontage* FireAnimation;

    // ������ �ִϸ��̼� ��Ÿ��
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
    class UAnimMontage* ReloadAnimation;

    UFUNCTION(Server, Reliable)
    // ������ �߻���Ű�� �Լ�
    void WeaponNoise(float Loudness);

protected:

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerCheckHit(FHitResult HitResult, float HitTime, FVector StartLocation, FVector EndDirection);

   
private:

    UPROPERTY()
    float HitThreshold;
};