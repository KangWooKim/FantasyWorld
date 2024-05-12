#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "BloodyGround/Component/ServerLocationComponent.h"
#include "BaseZombie.generated.h"

UENUM(BlueprintType)
enum class EZombieState : uint8
{
    None    UMETA(DisplayName = "None"),
    Attacking  UMETA(DisplayName = "Attacking"),
    HitReact  UMETA(DisplayName = "HitReact"),
    Death  UMETA(DisplayName = "Death"),
    Sleep  UMETA(DiaPlayName = "Sleep"),
    MAX UMETA(DisplayName = "MAX")
};


UCLASS()
class BLOODYGROUND_API ABaseZombie : public ACharacter
{
    GENERATED_BODY()

public:
    ABaseZombie();

    UFUNCTION()
    virtual void TakeShot(FHitResultData HitResult, float WeaponDamage);

    FORCEINLINE EZombieState GetZombieState() { return ZombieState; }

    FORCEINLINE UServerLocationComponent* GetServerLocationComponent() { return ServerLocationComp; }

protected:

    virtual void BeginPlay() override;
    virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
    virtual void PostInitializeComponents() override;

    // �ǰ� �� ��� ó�� ����
    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
    void HandleDeath();
    virtual void Tick(float DeltaTime) override;

    UFUNCTION()
        void OnSeePlayer(APawn* Pawn);

    UFUNCTION()
    bool IsTargetInSight(APawn* Target);


    UFUNCTION()
        void RestartPatrol();

    UFUNCTION()
        void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

    UFUNCTION()
    virtual void Attack(APawn* Target);

    UFUNCTION(BlueprintCallable, Category = "Zombie|Combat")
     virtual  void ApplyDamageToTarget();

    UFUNCTION(BlueprintCallable, Category = "Zombie|Combat")
        void HitReactEnd();

    UFUNCTION(BlueprintCallable, Category = "Zombie|Combat")
        void DeathEnd();

    UPROPERTY(EditAnywhere)
    float Damage;

    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Zombie|Health")
        float Health;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Zombie|Combat")
        float AttackRange;  // ���ݰ��ɹ���

    UPROPERTY(EditDefaultsOnly, Category = "Zombie|AI")
        float PatrolRadius;

    UPROPERTY(EditDefaultsOnly, Category = "Zombie|AI")
        float TimeToSleep;

    UPROPERTY(Replicated)
     // ���� ���� ���� Ÿ��
     APawn* CurrentTarget;

    UPROPERTY(Replicated)
     EZombieState ZombieState;

    // ���� ��� �ð� (��)
    float AttackCooldown = 5.0f;

    FTimerHandle TimerHandle_HitReactEnd;

private:
    FVector GetRandomPatrolPoint();
    void WakeUp();
    void GoBackToSleep();

    // ���� �󵵸� �����ϱ� ���� Ÿ�̸�
    UPROPERTY()
    float LastAttackTime;

    UPROPERTY(VisibleAnywhere, Category = "Zombie|Components")
     UPawnSensingComponent* PawnSensingComp;

    UPROPERTY(Replicated)
    FVector PatrolPoint;

    UPROPERTY()
    // ���������� �÷��̾ ������ �ð�
    float LastSeenTime;

    UPROPERTY()
    class UServerLocationComponent* ServerLocationComp;

    UPROPERTY()
    FTimerHandle SleepTimerHandle;
};