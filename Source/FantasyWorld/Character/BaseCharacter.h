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
	// �⺻ ĳ���� �Ӽ� �� ������ ���� ������ �Լ�
	ABaseCharacter();

	// ������ ó�� �Լ�. ACharacter Ŭ������ TakeDamage �Լ��� �������̵�
	// DamageAmount: ������ ��, DamageEvent: ������ �̺�Ʈ ����, EventInstigator: �̺�Ʈ �߻� ��Ʈ�ѷ�, DamageCauser: ������ ���� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// ĳ���Ͱ� �׾��� �� �߻��ϴ� �̺�Ʈ. �ܺο��� ���� ����
	FOnDeathEnd OnDeathEnd;

	// �þ� ������ ������ �� ȣ��Ǵ� �Լ�. ������ �� ����Ʈ�� ������ ���� ���� �߰��մϴ�.
	UFUNCTION()
	void OnBeginCapture(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// �þ� �������� ���� ������ �� ȣ��Ǵ� �Լ�. ������ �� ����Ʈ���� ������ ���� ���� �����մϴ�.
	UFUNCTION()
	void OnEndCapture(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	// ���� ���� Ȥ�� ĳ���� ���� �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;

	// ĳ���Ͱ� �ǰݵǾ��� �� ó���ϴ� �Լ�. GetHit �Լ��� �������̵�
	// ImpactPoint: �ǰ� ����, Hitter: ������ ����
	UFUNCTION()
		virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// ������ ó�� �Լ�
	// DamageAmount: ������ ��
	UFUNCTION()
		virtual void HandleDamage(float DamageAmount);

	// HUD�� ü�� ǥ�ø� ������Ʈ�ϴ� �Լ�
	UFUNCTION()
		void SetHUDHealth();

	// ĳ���Ͱ� ����ִ��� Ȯ���ϴ� �Լ�
	UFUNCTION()
		bool IsAlive();

	// �ǰ� ���� ��� �Լ�
	// ImpactPoint: �ǰ� ����
	UFUNCTION()
		void PlayHitSound(const FVector& ImpactPoint);

	// �ǰ� ��ƼŬ ���� �Լ�
	// ImpactPoint: �ǰ� ����
	UFUNCTION()
		void SpawnHitParticles(const FVector& ImpactPoint);

	// �⺻ ���� ���� �Լ�
	UFUNCTION()
		void SpawnDefaultWeapon();

	// ���� �浹 ���� ���� �Լ�. Blueprint���� ȣ�� ����
	// CollisionEnabled: �浹 ����
	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// ���� ���� ���� ����. VisibleAnywhere�� �ܺο��� �� �� ����
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;

	// �� ��° ���� ����. �⺻���� nullptr
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeaponSecond = nullptr;

	// ���� Ŭ����. �����Ϳ��� ���� ����
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	// �״� �Լ�. Blueprint������ ���� ����
	// BlueprintNativeEvent Ű���� ���
	UFUNCTION(BlueprintNativeEvent)
		void Die();

	// ĳ������ �������� ���� �޼���. ������ �̵�
	// Value: �̵��� ��
	void MoveForward(float Value);
	// ĳ������ �������� ���� �޼���. ���������� �̵�
	// Value: �̵��� ��
	void MoveRight(float Value);
	// ���� ���� �Լ�
	void StartJump();
	// ���� ���� �Լ�
	void StopJump();
	// ĳ������ ȸ���� ���� �޼���. ���콺 X�� �̵��� ���� ȸ��
	// Value: ȸ���� ��
	void Turn(float Value);
	// ĳ������ �Ӹ� ������ ���� �޼���. ���콺 Y�� �̵��� ���� �Ӹ� ���� ����
	// Value: �Ӹ� ������ ������ ��
	void LookUp(float Value);


	// �ʻ�� ���� ��ȯ�ϴ� �Լ�
	UFUNCTION()
		virtual void LethalMode();

	// ���� ���� �Լ�
	UFUNCTION()
		virtual void Attack();

	// �ʻ�� ��带 �����ϴ� �Լ�
	UFUNCTION()
		virtual void LethalModeFinish();

	// ���� ���� ���� (����â���� Ȯ�� ����)
	// Blueprint���� �б� ����
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

	// �Ӽ� ������Ʈ
	// �ܺο��� �б⸸ �����ϵ��� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UAttributeComponent* Attributes;

	// ������ ������ �Լ�. Blueprint���� ȣ�� ����
	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	// ��ȥ�� �߰��ϴ� �Լ�. �������̵� ��
	virtual void AddSouls(ASoul* Soul) override;

	// ġ���� ���¸� ����. Blueprint���� �б�/���� ����
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ELethalState LethalState = ELethalState::ELS_Off;

	// ������ ������ �Լ�. Blueprint���� ȣ�� ����
	UFUNCTION(BlueprintCallable)
		virtual void DeathEnd();

	// ��Ʈ ������ ������ �Լ�. Blueprint���� ȣ�� ����
	UFUNCTION(BlueprintCallable)
		virtual void HitReactEnd();

	// �Ͽ� �Լ�
	UFUNCTION()
		void LockOnTarget();

	// ���� ����� ���� ã�� �Լ�
	UFUNCTION()
		AActor* GetClosestEnemy();

	// ���� ��� Ÿ���� ����
	UPROPERTY()
		AActor* CurrentLockedTarget;

	// �׼� ���¸� ����. Blueprint���� �б�/���� ����
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	// ���� �������� Ȯ���ϴ� �Լ�
	virtual bool CanAttack();

	// ��Ʈ ���� ��Ÿ�ָ� ����ϴ� �Լ�
	// SectionName: ��Ÿ�� ���� �̸�
	void PlayHitReactMontage(const FName& SectionName);

	// ���� ��Ÿ�ָ� ����ϴ� �Լ�
	virtual int32 PlayAttackMontage();

	// ���� ��Ÿ�ָ� ����ϴ� �Լ�
	virtual int32 PlayDeathMontage();

	// ���� ��Ÿ�ָ� �����ϴ� �Լ�
	void StopAttackMontage();

	// ���⼺�� ���� ��Ʈ ����
	// ImpactPoint: �ǰ� ����
	void DirectionalHitReact(const FVector& ImpactPoint);

	// ���� �������� UI ������Ʈ
	UPROPERTY()
		class UCombatOverlay* CombatOverlay;

public:
	// �����Ӹ��� ȣ��Ǵ� �Լ�
	virtual void Tick(float DeltaTime) override;

	// ��ü �ı� �Լ�
	virtual void Destroy();

	// �Է°� �Լ��� �����ϴ� �Լ�
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �׼� ���� getter �Լ�
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	// ġ���� ���� getter �Լ�
	FORCEINLINE ELethalState GetLethalState() const { return LethalState; }

	// ���� ���� getter �Լ�
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

	// �÷��̾� ��Ʈ�ѷ��� �������� �Լ�
	UFUNCTION()
		AFantasyPlayerController* GetPlayerController();

	// �޴��� ���� �Լ�
	UFUNCTION()
		void OpenMenu();



private:
	// ��Ÿ���� Ư�� ������ ����ϴ� �Լ�
	// Montage: ����� �ִϸ��̼� ��Ÿ��
	// SectionName: ����� ��Ÿ�� ������ �̸�
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	// ������ ��Ÿ�� ������ ����ϴ� �Լ�
	// Montage: ����� �ִϸ��̼� ��Ÿ��
	// SectionNames: ����� �� �ִ� ��Ÿ�� ������ �̸� ���
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// ī�޶� �� ������Ʈ. ĳ���Ϳ� ī�޶� ������ �Ÿ��� �����մϴ�.
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	// ĳ���Ͱ� ����ϴ� �� ī�޶� ������Ʈ.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// ��Ʈ ���� �Ӽ�. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitSound;

	// ��Ʈ ����Ʈ ��ƼŬ. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = Combat)
		UParticleSystem* HitParticles;

	// ���� ��Ÿ��. ����â���� �⺻���� ������ �� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* AttackMontage;

	// ��Ʈ ���� ��Ÿ��. ����â���� �⺻���� ������ �� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* HitReactMontage;

	// ���� ��Ÿ��. ����â���� �⺻���� ������ �� ����.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DeathMontage;

	// ���� ��Ÿ�� ������ �̸� ���. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> AttackMontageSections;

	// ���� �� ī�޶� ��鸲 Ŭ����. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> CombatCameraShake;

	// ���� ����. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* DeathSound;

	// �ʻ�� ��� ����. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* LethalModeSound;

	// �ʻ�� ��� ��ƼŬ. ����â���� ���� ����.
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* LethalModeParticle;

	// �÷��̾� ��Ʈ�ѷ� Ŭ������ �ν��Ͻ�
	UPROPERTY()
		class AFantasyPlayerController* PlayerController;

	// ���� �ν��Ͻ� Ŭ������ �ν��Ͻ�
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// �Ϲ� ���� HUD Ŭ������ �ν��Ͻ�
	UPROPERTY()
		class ANormalLevelHUD* HUD;

	// �Ͽ� ��� ���θ� �����ϴ� �Ҹ� ��
	UPROPERTY()
		bool bUseLockOnTarget = false;

	// �Ͽ� ���� ����� �þ� ���� �Դϴ�. �� ������ ���� �Ͽ��� �����մϴ�.
	UPROPERTY(EditAnywhere, Category = "LockOn")
	class USphereComponent* LockOnRange;
	
	// ���� ���� �Ͽ� ���� ���� �ִ� �����Դϴ�.
	UPROPERTY()
	TSet<AActor*> CapturedEnemies;
};
