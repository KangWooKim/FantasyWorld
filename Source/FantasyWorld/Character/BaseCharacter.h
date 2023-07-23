#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "FantasyWorld/HitInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class UCameraShakeBase;

UCLASS()
class FANTASYWORLD_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;
	virtual void HandleDamage(float DamageAmount);
	void SetHUDHealth();
	bool IsAlive();
	void PlayHitSound(const FVector& ImpactPoint);
	void SpawnHitParticles(const FVector& ImpactPoint);

	void SpawnDefaultWeapon();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, Category = Weapon)
	AWeapon* EquippedWeaponSecond = nullptr;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AWeapon> WeaponClass;

	UFUNCTION(BlueprintNativeEvent)
	void Die();

	// 캐릭터의 움직임을 위한 메서드 입니다.
	void MoveForward(float Value);
	void MoveRight(float Value);
	void StartJump();
	void StopJump();
	void Turn(float Value);
	void LookUp(float Value);

	void LethalMode();
	void Attack();
	void LethalModeFinish();
	// 죽음 포즈 (편집창에서 확인 가능)
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAttributeComponent* Attributes;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();
	

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	virtual bool CanAttack();
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void StopAttackMontage();
	void DirectionalHitReact(const FVector& ImpactPoint);
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	FORCEINLINE ELethalState GetLethalState() const { return LethalState; }

	// 죽음 포즈 getter 함수
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }


private : 

	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// 카메라 붐에 대한 속성입니다. 이는 게임 엔진에서 카메라와 캐릭터 사이의 거리를 조절합니다.
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	// 캐릭터가 보는 카메라에 대한 속성입니다.
	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;

	UPROPERTY(EditAnywhere, Category = Combat)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* HitParticles;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ELethalState LethalState = ELethalState::ELS_Off;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* HitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combat")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, Category = "Comabat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TSubclassOf<UCameraShakeBase> CombatCameraShake;

	class AFantasyPlayerController* PlayerController;

	
};
