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
    // 기본 생성자
    ABaseEnemy();
    // Tick 함수 오버라이딩. 매 프레임마다 호출됩니다.
    virtual void Tick(float DeltaTime) override;
    // 죽음 이벤트에 대한 델리게이트(Delegate)
    FEnemyDeath EnemyDeath;

protected:
    // BeginPlay 함수 오버라이딩. 시작 시 한 번만 호출됩니다.
    virtual void BeginPlay() override;

    // Combat (전투) 관련 메서드들

    // 히트 이벤트 처리
    virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
    // 공격 처리
    virtual void Attack();

    // 블루프린트에서 사용 가능한 Die 이벤트. 죽음 처리를 담당합니다.
    UFUNCTION(BlueprintNativeEvent)
        void Die();

    // 방향에 따른 히트 반응 처리
    void DirectionalHitReact(const FVector& ImpactPoint);
    // 데미지 처리
    virtual void HandleDamage(float DamageAmount);
    // 히트 사운드 재생
    void PlayHitSound(const FVector& ImpactPoint);
    // 히트 파티클 생성
    void SpawnHitParticles(const FVector& ImpactPoint);
    // 캡슐 충돌 비활성화
    void DisableCapsule();
    // 공격 가능 여부 판단
    virtual bool CanAttack();
    // 생존 여부 확인
    bool IsAlive();
    // 메시 충돌 비활성화
    void DisableMeshCollision();

    // 애니메이션 (Montage) 관련 메서드들

    // 히트 반응 애니메이션 재생
    void PlayHitReactMontage(const FName& SectionName);
    // 공격 애니메이션 재생
    virtual int32 PlayAttackMontage();
    // 죽음 애니메이션 재생
    virtual int32 PlayDeathMontage();
    // 회피 애니메이션 재생
    virtual void PlayDodgeMontage();
    // 공격 애니메이션 중지
    void StopAttackMontage();

    // 블루프린트에서 호출 가능한 함수들

    UFUNCTION(BlueprintCallable)
        FVector GetTranslationWarpTarget();  // 위치 이동 대상 가져오기

    UFUNCTION(BlueprintCallable)
        FVector GetRotationWarpTarget();  // 회전 대상 위치 가져오기

    UFUNCTION(BlueprintCallable)
        virtual void AttackEnd();  // 공격 종료 처리

    UFUNCTION(BlueprintCallable)
        virtual void DodgeEnd();  // 회피 종료 처리

    UFUNCTION(BlueprintCallable)
        void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);  // 무기 충돌 설정

    // 무기, 속성, 전투 대상 등의 프로퍼티들

    UPROPERTY(VisibleAnywhere, Category = Weapon)
        AWeapon* EquippedWeapon;  // 장착된 무기

    UPROPERTY(VisibleAnywhere, Category = Weapon)
        AWeapon* EquippedWeaponSecond = nullptr;  // 장착된 두 번째 무기 (선택적)

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
        UAttributeComponent* Attributes;  // 속성 컴포넌트

    UPROPERTY(BlueprintReadOnly, Category = Combat)
        AActor* CombatTarget;  // 전투 대상

    UPROPERTY(EditAnywhere, Category = Combat)
        double WarpTargetDistance = 75.f;  // 목표까지의 이동 거리

    UPROPERTY(BlueprintReadOnly)
        TEnumAsByte<EDeathPose> DeathPose;  // 죽음 포즈

    
private:
    // 애니메이션 몽타주의 특정 섹션을 재생합니다.
    void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
    // 여러 몽타주 섹션 중 하나를 랜덤으로 선택해 재생합니다.
    int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

    // 전투 관련 사운드와 파티클, 애니메이션 몽타주를 저장하는 프로퍼티들
    UPROPERTY(EditAnywhere, Category = Combat)
        USoundBase* HitSound;  // 히트 사운드

    UPROPERTY(EditAnywhere, Category = Combat)
        USoundBase* DeathSound;  // 죽음 사운드

    UPROPERTY(EditAnywhere, Category = Combat)
        UParticleSystem* HitParticles;  // 히트 시 생성되는 파티클

    // 애니메이션 몽타주들
    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* AttackMontage;  // 공격 애니메이션

    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* HitReactMontage;  // 히트 반응 애니메이션

    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* DeathMontage;  // 죽음 애니메이션

    UPROPERTY(EditDefaultsOnly, Category = Combat)
        UAnimMontage* DodgeMontage;  // 회피 애니메이션

    // 몽타주 섹션 이름들 (어떤 부분을 재생할지 결정할 때 사용)
    UPROPERTY(EditAnywhere, Category = Combat)
        TArray<FName> AttackMontageSections;

    UPROPERTY(EditAnywhere, Category = Combat)
        TArray<FName> DeathMontageSections;

    
public:
    // 죽음 포즈를 가져오는 인라인 함수
    FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

};
