// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FantasyWorld/HitInterface.h"
#include "FantasyWorld/Character/CharacterTypes.h"
#include "BaseEnemy.generated.h"

class AWeapon;
class UAttributeComponent;
class UAnimMontage;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEnemyDeath);

UCLASS()
class FANTASYWORLD_API ABaseEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

   
public:
    // �⺻ ������
    ABaseEnemy();
    // Tick �Լ� �������̵�. �� �����Ӹ��� ȣ��˴ϴ�.
    virtual void Tick(float DeltaTime) override;
    // ���� �̺�Ʈ�� ���� ��������Ʈ(Delegate)
    FEnemyDeath EnemyDeath;

protected:
    // BeginPlay �Լ� �������̵�. ���� �� �� ���� ȣ��˴ϴ�.
    virtual void BeginPlay() override;

    // Combat (����) ���� �޼����

    // ��Ʈ �̺�Ʈ ó��
    virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
    // ���� ó��
    virtual void Attack();

    // �������Ʈ���� ��� ������ Die �̺�Ʈ. ���� ó���� ����մϴ�.
    UFUNCTION(BlueprintNativeEvent)
        void Die();

    // ���⿡ ���� ��Ʈ ���� ó��
    void DirectionalHitReact(const FVector& ImpactPoint);
    // ������ ó��
    virtual void HandleDamage(float DamageAmount);
    // ��Ʈ ���� ���
    void PlayHitSound(const FVector& ImpactPoint);
    // ��Ʈ ��ƼŬ ����
    void SpawnHitParticles(const FVector& ImpactPoint);
    // ĸ�� �浹 ��Ȱ��ȭ
    void DisableCapsule();
    // ���� ���� ���� �Ǵ�
    virtual bool CanAttack();
    // ���� ���� Ȯ��
    bool IsAlive();
    // �޽� �浹 ��Ȱ��ȭ
    void DisableMeshCollision();

    // �ִϸ��̼� (Montage) ���� �޼����

    // ��Ʈ ���� �ִϸ��̼� ���
    void PlayHitReactMontage(const FName& SectionName);
    // ���� �ִϸ��̼� ���
    virtual int32 PlayAttackMontage();
    // ���� �ִϸ��̼� ���
    virtual int32 PlayDeathMontage();
    // ȸ�� �ִϸ��̼� ���
    virtual void PlayDodgeMontage();
    // ���� �ִϸ��̼� ����
    void StopAttackMontage();

    // �������Ʈ���� ȣ�� ������ �Լ���

    UFUNCTION(BlueprintCallable)
        FVector GetTranslationWarpTarget();  // ��ġ �̵� ��� ��������

    UFUNCTION(BlueprintCallable)
        FVector GetRotationWarpTarget();  // ȸ�� ��� ��ġ ��������

    UFUNCTION(BlueprintCallable)
        virtual void AttackEnd();  // ���� ���� ó��

    UFUNCTION(BlueprintCallable)
        virtual void DodgeEnd();  // ȸ�� ���� ó��

    UFUNCTION(BlueprintCallable)
        void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);  // ���� �浹 ����

    // ����, �Ӽ�, ���� ��� ���� ������Ƽ��

    UPROPERTY(VisibleAnywhere, Category = Weapon)
        AWeapon* EquippedWeapon;  // ������ ����

    UPROPERTY(VisibleAnywhere, Category = Weapon)
        AWeapon* EquippedWeaponSecond = nullptr;  // ������ �� ��° ���� (������)

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        UAttributeComponent* Attributes;  // �Ӽ� ������Ʈ

    UPROPERTY(BlueprintReadOnly, Category = Combat)
        AActor* CombatTarget;  // ���� ���

    UPROPERTY(EditAnywhere, Category = Combat)
        double WarpTargetDistance = 75.f;  // ��ǥ������ �̵� �Ÿ�

    UPROPERTY(BlueprintReadOnly)
        TEnumAsByte<EDeathPose> DeathPose;  // ���� ����

    
private:
    // �ִϸ��̼� ��Ÿ���� Ư�� ������ ����մϴ�.
    void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
    // ���� ��Ÿ�� ���� �� �ϳ��� �������� ������ ����մϴ�.
    int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

    // ���� ���� ����� ��ƼŬ, �ִϸ��̼� ��Ÿ�ָ� �����ϴ� ������Ƽ��
    UPROPERTY(EditAnywhere, Category = Combat)
        USoundBase* HitSound;  // ��Ʈ ����

    UPROPERTY(EditAnywhere, Category = Combat)
        USoundBase* DeathSound;  // ���� ����

    UPROPERTY(EditAnywhere, Category = Combat)
        UParticleSystem* HitParticles;  // ��Ʈ �� �����Ǵ� ��ƼŬ

    // �ִϸ��̼� ��Ÿ�ֵ�
    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* AttackMontage;  // ���� �ִϸ��̼�

    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* HitReactMontage;  // ��Ʈ ���� �ִϸ��̼�

    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* DeathMontage;  // ���� �ִϸ��̼�

    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* DodgeMontage;  // ȸ�� �ִϸ��̼�

    // ��Ÿ�� ���� �̸��� (� �κ��� ������� ������ �� ���)
    UPROPERTY(EditAnywhere, Category = Combat)
        TArray<FName> AttackMontageSections;

    UPROPERTY(EditAnywhere, Category = Combat)
        TArray<FName> DeathMontageSections;

    
public:
    // ���� ��� �������� �ζ��� �Լ�
    FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

};
