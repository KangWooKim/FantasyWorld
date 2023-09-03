// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UNiagaraComponent;


enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_Equipped
};


UCLASS()
class FANTASYWORLD_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	// Ŭ������ �⺻ �Ӽ��� �����ϴ� ������
	AItem();

protected:
	// ���� ���� �Ǵ� ��ü�� ������ �� ȣ��Ǵ� �Լ�
	virtual void BeginPlay() override;

	// ���� �ĵ��� ������ �����ϴ� �Ӽ� (Blueprint���� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float Amplitude = 0.25f;

	// ���� �ĵ��� �ð� ����� �����ϴ� �Ӽ� (Blueprint���� ���� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float TimeConstant = 5.f;

	// ��ȯ�� ���� ���� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintPure)
		float TransformedSin();

	// ��ȯ�� �ڻ��� ���� ��ȯ�ϴ� �Լ�
	UFUNCTION(BlueprintPure)
		float TransformedCos();

	// �� ���� ����� ����ϴ� ���ø� �Լ�
	template<typename T>
	T Avg(T First, T Second);

	// ���Ǿ� ������Ʈ�� �ٸ� ���Ͱ� ������ �� ȣ��Ǵ� �Լ�
	// OverlappedComponent: ��ģ ������Ʈ
	// OtherActor: ��ģ �ٸ� ����
	// OtherComp: ��ģ �ٸ� ������Ʈ
	// OtherBodyIndex: ��ģ �ٸ� ������Ʈ�� �ٵ� �ε���
	// bFromSweep: Sweep �������� ���� ��ħ ����
	// SweepResult: Sweep ���� ���
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ���Ǿ� ������Ʈ�� �ٸ� ���Ͱ� ��ħ�� ������ �� ȣ��Ǵ� �Լ�
	// OverlappedComponent: ��ģ ������Ʈ
	// OtherActor: ��ģ �ٸ� ����
	// OtherComp: ��ģ �ٸ� ������Ʈ
	// OtherBodyIndex: ��ģ �ٸ� ������Ʈ�� �ٵ� �ε���
	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// �������� ���� �� ȿ���� �����ϴ� �Լ�
	virtual void SpawnPickupSystem();

	// �������� ���� �� �Ҹ��� �����ϴ� �Լ�
	virtual void SpawnPickupSound();

	// �������� �޽� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ItemMesh;

	// �������� ���� (��: ȣ���� ��)
	EItemState ItemState = EItemState::EIS_Hovering;

	// �������� �浹 ������ ��Ÿ���� ���Ǿ� ������Ʈ
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;

	// �������� Ư�� ȿ�� (��ƼŬ)
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* ItemEffect;

	// �������� �Ծ��� ���� �Ҹ�
	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;


public:
	// �� �����Ӹ��� ȣ��Ǵ� �Լ�
	virtual void Tick(float DeltaTime) override;

private:
	// ���������� ���Ǵ�, ���� �ð��� �����ϴ� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunningTime;

	// �������� �Ծ��� ���� ��ƼŬ ȿ��
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* PickupEffect;


};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
