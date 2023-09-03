// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Soul.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ASoul : public AItem
{
	GENERATED_BODY()
	
public:
    // Tick �޼��� �������̵�, �����Ӹ��� ȣ���
    virtual void Tick(float DeltaTime) override;

protected:
    // BeginPlay �޼��� �������̵�, ���Ͱ� ������ �� �� ���� ȣ���
    virtual void BeginPlay() override;

    // ��ü ������Ʈ�� ������ �� ȣ��Ǵ� �޼���
    // OverlappedComponent: ��ģ ������Ʈ
    // OtherActor: �ٸ� ����
    // OtherComp: �ٸ� ������Ʈ
    // OtherBodyIndex: �ٸ� ������Ʈ�� �ٵ� �ε���
    // bFromSweep: ������ ���� ��ģ ���
    // SweepResult: ���� ���
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
    // Souls ����, �����Ϳ��� ���� ����
    // �ҿ��� ���� ����
    UPROPERTY(EditAnywhere, Category = "Soul Properties")
        int32 Souls;

    // DesiredZ ����, ���Ͱ� �̵��ؾ� �� Z ��ġ
    float DesiredZ;

    // DriftRate ����, �����Ϳ��� ���� ����
    // ���Ͱ� �̵��ϴ� �ӵ� (����: cm/s)
    UPROPERTY(EditAnywhere)
        double DriftRate = -15.f;

public:
    // Souls ������ �������� �ζ��� �޼���
    FORCEINLINE int32 GetSouls() const { return Souls; }

    // Souls ������ �����ϴ� �ζ��� �޼���, NumberOfSouls�� ������ �ҿ��� ��
    FORCEINLINE void  SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }


};
