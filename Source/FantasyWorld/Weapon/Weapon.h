// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

class UBoxComponent;
class USoundBase;


UCLASS()
class FANTASYWORLD_API AWeapon : public AActor
{
	GENERATED_BODY()
	
		// ��� ���Ͽ��� AWeapon Ŭ���� ���� �κ�

public:
	AWeapon();  // �⺻ ������

	// ���⸦ �����ϴ� �Լ�
	// InParent: ���Ⱑ ������ �θ� ������Ʈ
	// InSocketName: ������ ������ �̸�
	// NewOwner: ������ ���ο� ����
	// NewInstigator: ���⸦ ����ϴ� ĳ����
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void PlayEquipSound();  // ���⸦ ������ ���� ���带 ����ϴ� �Լ�

	// ���� �޽ø� Ư�� ���Ͽ� �����ϴ� �Լ�
	// InParent: ���Ⱑ ������ �θ� ������Ʈ
	// InSocketName: ������ ������ �̸�
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	// ������ ���� ���
	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;  // ���� ���� �� ȣ��Ǵ� �Լ�

	// ������ �߻� �� ȣ��Ǵ� �Լ�
	// OverlappedComponent: �������� �߻��� ������Ʈ
	// OtherActor: �������� �߻��� �ٸ� ����
	// OtherComp: �������� �߻��� �ٸ� ������Ʈ
	// OtherBodyIndex: �������� ������Ʈ�� �ε���
	// bFromSweep: Sweep ����� �������� �߻��ߴ��� ����
	// SweepResult: ������ ���
	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ������ Ÿ���� ������ Ȯ���ϴ� �Լ�
	// OtherActor: Ȯ���� �ٸ� ����
	bool ActorIsSameType(AActor* OtherActor);

	// ��Ʈ ó�� ������ �����ϴ� �Լ�
	// BoxHit: ��Ʈ ��� ����
	void ExecuteGetHit(FHitResult& BoxHit);

	// �������Ʈ���� ���� ������ �̺�Ʈ
	UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);

	// ������ �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* WeaponMesh;

private:
	// ���� ������ �����ϴ� �Լ�
	// BoxHit: ���� ��� ����
	void BoxTrace(FHitResult& BoxHit);

	// ���� ���� ����
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		FVector BoxTraceExtent = FVector(5.f);

	// ����� ǥ�� ����
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		bool bShowBoxDebug = false;

	// ���� ����
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		USoundBase* EquipSound;

	// ������ �ڽ� ������Ʈ
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		UBoxComponent* WeaponBox;

	// ���� ���� ���� ��ġ
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceStart;

	// ���� ���� ���� ��ġ
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceEnd;

	// ������ ������
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float Damage = 20.f;

public:
	// ���� �ڽ� ������Ʈ�� ��ȯ�ϴ� �ζ��� �Լ�
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	// ���� �������� �����ϴ� �ζ��� �Լ�
	FORCEINLINE void SetWeaponDamage(float WeaponDamage) { Damage = WeaponDamage; }


};
