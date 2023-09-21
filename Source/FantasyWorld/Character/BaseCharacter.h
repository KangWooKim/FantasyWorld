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
	// 기본 캐릭터 속성 값 설정을 위한 생성자 함수
	ABaseCharacter();

	// 데미지 처리 함수. ACharacter 클래스의 TakeDamage 함수를 오버라이딩
	// DamageAmount: 데미지 양, DamageEvent: 데미지 이벤트 정보, EventInstigator: 이벤트 발생 컨트롤러, DamageCauser: 데미지 원인 액터
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// 캐릭터가 죽었을 때 발생하는 이벤트. 외부에서 구독 가능
	FOnDeathEnd OnDeathEnd;

	// 시야 범위에 들어왔을 때 호출되는 함수. 포착된 적 리스트에 범위에 들어온 적을 추가합니다.
	UFUNCTION()
	void OnBeginCapture(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 시야 범위에서 적이 나갔을 때 호출되는 함수. 포착된 적 리스트에서 범위에 나간 적을 제거합니다.
	UFUNCTION()
	void OnEndCapture(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:
	// Called when the game starts or when spawned
	// 게임 시작 혹은 캐릭터 생성 시 호출되는 함수
	virtual void BeginPlay() override;

	// 캐릭터가 피격되었을 때 처리하는 함수. GetHit 함수를 오버라이딩
	// ImpactPoint: 피격 지점, Hitter: 공격한 액터
	UFUNCTION()
		virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// 데미지 처리 함수
	// DamageAmount: 데미지 양
	UFUNCTION()
		virtual void HandleDamage(float DamageAmount);

	// HUD의 체력 표시를 업데이트하는 함수
	UFUNCTION()
		void SetHUDHealth();

	// 캐릭터가 살아있는지 확인하는 함수
	UFUNCTION()
		bool IsAlive();

	// 피격 사운드 재생 함수
	// ImpactPoint: 피격 지점
	UFUNCTION()
		void PlayHitSound(const FVector& ImpactPoint);

	// 피격 파티클 생성 함수
	// ImpactPoint: 피격 지점
	UFUNCTION()
		void SpawnHitParticles(const FVector& ImpactPoint);

	// 기본 무기 생성 함수
	UFUNCTION()
		void SpawnDefaultWeapon();

	// 무기 충돌 상태 설정 함수. Blueprint에서 호출 가능
	// CollisionEnabled: 충돌 상태
	UFUNCTION(BlueprintCallable)
		void SetWeaponCollisionEnabled(ECollisionEnabled::Type CollisionEnabled);

	// 현재 장착 중인 무기. VisibleAnywhere로 외부에서 볼 수 있음
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeapon;

	// 두 번째 장착 무기. 기본값은 nullptr
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		AWeapon* EquippedWeaponSecond = nullptr;

	// 무기 클래스. 에디터에서 설정 가능
	UPROPERTY(EditAnywhere)
		TSubclassOf<class AWeapon> WeaponClass;

	// 죽는 함수. Blueprint에서도 구현 가능
	// BlueprintNativeEvent 키워드 사용
	UFUNCTION(BlueprintNativeEvent)
		void Die();

	// 캐릭터의 움직임을 위한 메서드. 앞으로 이동
	// Value: 이동할 값
	void MoveForward(float Value);
	// 캐릭터의 움직임을 위한 메서드. 오른쪽으로 이동
	// Value: 이동할 값
	void MoveRight(float Value);
	// 점프 시작 함수
	void StartJump();
	// 점프 중지 함수
	void StopJump();
	// 캐릭터의 회전을 위한 메서드. 마우스 X축 이동에 따른 회전
	// Value: 회전할 값
	void Turn(float Value);
	// 캐릭터의 머리 방향을 위한 메서드. 마우스 Y축 이동에 따른 머리 방향 변경
	// Value: 머리 방향을 변경할 값
	void LookUp(float Value);


	// 필살기 모드로 전환하는 함수
	UFUNCTION()
		virtual void LethalMode();

	// 공격 수행 함수
	UFUNCTION()
		virtual void Attack();

	// 필살기 모드를 종료하는 함수
	UFUNCTION()
		virtual void LethalModeFinish();

	// 죽음 포즈 상태 (편집창에서 확인 가능)
	// Blueprint에서 읽기 전용
	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EDeathPose> DeathPose;

	// 속성 컴포넌트
	// 외부에서 읽기만 가능하도록 설정
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UAttributeComponent* Attributes;

	// 공격을 끝내는 함수. Blueprint에서 호출 가능
	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	// 영혼을 추가하는 함수. 오버라이딩 됨
	virtual void AddSouls(ASoul* Soul) override;

	// 치명적 상태를 저장. Blueprint에서 읽기/쓰기 가능
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		ELethalState LethalState = ELethalState::ELS_Off;

	// 죽음을 끝내는 함수. Blueprint에서 호출 가능
	UFUNCTION(BlueprintCallable)
		virtual void DeathEnd();

	// 히트 반응을 끝내는 함수. Blueprint에서 호출 가능
	UFUNCTION(BlueprintCallable)
		virtual void HitReactEnd();

	// 록온 함수
	UFUNCTION()
		void LockOnTarget();

	// 가장 가까운 적을 찾는 함수
	UFUNCTION()
		AActor* GetClosestEnemy();

	// 현재 잠긴 타겟을 저장
	UPROPERTY()
		AActor* CurrentLockedTarget;

	// 액션 상태를 저장. Blueprint에서 읽기/쓰기 가능
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
		EActionState ActionState = EActionState::EAS_Unoccupied;

	// 공격 가능한지 확인하는 함수
	virtual bool CanAttack();

	// 히트 반응 몽타주를 재생하는 함수
	// SectionName: 몽타주 섹션 이름
	void PlayHitReactMontage(const FName& SectionName);

	// 공격 몽타주를 재생하는 함수
	virtual int32 PlayAttackMontage();

	// 죽음 몽타주를 재생하는 함수
	virtual int32 PlayDeathMontage();

	// 공격 몽타주를 중지하는 함수
	void StopAttackMontage();

	// 방향성을 가진 히트 반응
	// ImpactPoint: 피격 지점
	void DirectionalHitReact(const FVector& ImpactPoint);

	// 전투 오버레이 UI 컴포넌트
	UPROPERTY()
		class UCombatOverlay* CombatOverlay;

public:
	// 프레임마다 호출되는 함수
	virtual void Tick(float DeltaTime) override;

	// 객체 파괴 함수
	virtual void Destroy();

	// 입력과 함수를 연결하는 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 액션 상태 getter 함수
	FORCEINLINE EActionState GetActionState() const { return ActionState; }

	// 치명적 상태 getter 함수
	FORCEINLINE ELethalState GetLethalState() const { return LethalState; }

	// 죽음 포즈 getter 함수
	FORCEINLINE TEnumAsByte<EDeathPose> GetDeathPose() const { return DeathPose; }

	// 플레이어 컨트롤러를 가져오는 함수
	UFUNCTION()
		AFantasyPlayerController* GetPlayerController();

	// 메뉴를 여는 함수
	UFUNCTION()
		void OpenMenu();



private:
	// 몽타주의 특정 섹션을 재생하는 함수
	// Montage: 재생할 애니메이션 몽타주
	// SectionName: 재생할 몽타주 섹션의 이름
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);

	// 랜덤한 몽타주 섹션을 재생하는 함수
	// Montage: 재생할 애니메이션 몽타주
	// SectionNames: 재생할 수 있는 몽타주 섹션의 이름 목록
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	// 카메라 붐 컴포넌트. 캐릭터와 카메라 사이의 거리를 조절합니다.
	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* CameraBoom;

	// 캐릭터가 사용하는 뷰 카메라 컴포넌트.
	UPROPERTY(VisibleAnywhere)
		UCameraComponent* ViewCamera;

	// 히트 사운드 속성. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = Combat)
		USoundBase* HitSound;

	// 히트 이펙트 파티클. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = Combat)
		UParticleSystem* HitParticles;

	// 공격 몽타주. 편집창에서 기본값을 설정할 수 있음.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* AttackMontage;

	// 히트 반응 몽타주. 편집창에서 기본값을 설정할 수 있음.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* HitReactMontage;

	// 죽음 몽타주. 편집창에서 기본값을 설정할 수 있음.
	UPROPERTY(EditDefaultsOnly, Category = "Combat")
		UAnimMontage* DeathMontage;

	// 공격 몽타주 섹션의 이름 목록. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = "Combat")
		TArray<FName> AttackMontageSections;

	// 전투 중 카메라 흔들림 클래스. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = "Combat")
		TSubclassOf<UCameraShakeBase> CombatCameraShake;

	// 죽음 사운드. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* DeathSound;

	// 필살기 모드 사운드. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = "Combat")
		USoundBase* LethalModeSound;

	// 필살기 모드 파티클. 편집창에서 변경 가능.
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* LethalModeParticle;

	// 플레이어 컨트롤러 클래스의 인스턴스
	UPROPERTY()
		class AFantasyPlayerController* PlayerController;

	// 게임 인스턴스 클래스의 인스턴스
	UPROPERTY()
		UMyGameInstance* GameInstance;

	// 일반 레벨 HUD 클래스의 인스턴스
	UPROPERTY()
		class ANormalLevelHUD* HUD;

	// 록온 사용 여부를 저장하는 불린 값
	UPROPERTY()
		bool bUseLockOnTarget = false;

	// 록온 사용시 사용할 시야 범위 입니다. 이 내부의 적만 록온이 가능합니다.
	UPROPERTY(EditAnywhere, Category = "LockOn")
	class USphereComponent* LockOnRange;
	
	// 지금 현재 록온 범위 내에 있는 적들입니다.
	UPROPERTY()
	TSet<AActor*> CapturedEnemies;
};
