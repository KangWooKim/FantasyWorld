#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "FantasyWorld/HitInterface.h"
#include "FantasyWorld/PickupInterface.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AWeapon;
class UCameraShakeBase;
class AFantasyPlayerController;
class UMyGameInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathEnd);

UCLASS()
class FANTASYWORLD_API ABaseCharacter : public ACharacter, public IHitInterface, public IPickupInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	FOnDeathEnd OnDeathEnd;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	UFUNCTION()
	virtual void HandleDamage(float DamageAmount);

	UFUNCTION()
	void SetHUDHealth();

	UFUNCTION()
	bool IsAlive();

	UFUNCTION()
	void PlayHitSound(const FVector& ImpactPoint);

	UFUNCTION()
	void SpawnHitParticles(const FVector& ImpactPoint);

	UFUNCTION()
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

	UFUNCTION()
	virtual void LethalMode();

	UFUNCTION()
	virtual void Attack();

	UFUNCTION()
	virtual void LethalModeFinish();
	// 죽음 포즈 (편집창에서 확인 가능)
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<EDeathPose> DeathPose;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UAttributeComponent* Attributes;

	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual void AddSouls(ASoul* Soul) override;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ELethalState LethalState = ELethalState::ELS_Off;
	

	UFUNCTION(BlueprintCallable)
	virtual void DeathEnd();

	UFUNCTION(BlueprintCallable)
	virtual void HitReactEnd();

	UFUNCTION()
	void LockOnTarget();

	UFUNCTION()
	AActor* GetClosestEnemy();

	UPROPERTY()
	AActor* CurrentLockedTarget;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_Unoccupied;

	virtual bool CanAttack();
	void PlayHitReactMontage(const FName& SectionName);
	virtual int32 PlayAttackMontage();
	virtual int32 PlayDeathMontage();
	void StopAttackMontage();
	void DirectionalHitReact(const FVector& ImpactPoint);

	UPROPERTY()
	class UCombatOverlay* CombatOverlay;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Destroy();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE EActionState GetActionState() const { return ActionState; }
	FORCEINLINE ELethalState GetLethalState() const { return LethalState; }

	// 죽음 포즈 getter 함수
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

	UFUNCTION()
	AFantasyPlayerController* GetPlayerController();

	UFUNCTION()
	void OpenMenu();


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

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	USoundBase* LethalModeSound;

	UPROPERTY(EditAnywhere, Category = "Combat")
	UParticleSystem* LethalModeParticle;

	UPROPERTY()
	class AFantasyPlayerController* PlayerController;

	UPROPERTY()
	UMyGameInstance* GameInstance;

	UPROPERTY()
	class ANormalLevelHUD* HUD;

	UPROPERTY()
	bool bUseLockOnTarget = false;

	// Tutorial Level에서 움직임 조작법을 설명하기 위해 선언한 함수.

	
};
