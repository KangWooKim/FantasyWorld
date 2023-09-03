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
    // Tick 메서드 오버라이딩, 프레임마다 호출됨
    virtual void Tick(float DeltaTime) override;

protected:
    // BeginPlay 메서드 오버라이딩, 액터가 스폰될 때 한 번만 호출됨
    virtual void BeginPlay() override;

    // 구체 컴포넌트와 겹쳤을 때 호출되는 메서드
    // OverlappedComponent: 겹친 컴포넌트
    // OtherActor: 다른 액터
    // OtherComp: 다른 컴포넌트
    // OtherBodyIndex: 다른 컴포넌트의 바디 인덱스
    // bFromSweep: 스윕에 의해 겹친 경우
    // SweepResult: 스윕 결과
    virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
    // Souls 변수, 에디터에서 편집 가능
    // 소울의 수를 저장
    UPROPERTY(EditAnywhere, Category = "Soul Properties")
        int32 Souls;

    // DesiredZ 변수, 액터가 이동해야 할 Z 위치
    float DesiredZ;

    // DriftRate 변수, 에디터에서 편집 가능
    // 액터가 이동하는 속도 (단위: cm/s)
    UPROPERTY(EditAnywhere)
        double DriftRate = -15.f;

public:
    // Souls 변수를 가져오는 인라인 메서드
    FORCEINLINE int32 GetSouls() const { return Souls; }

    // Souls 변수를 설정하는 인라인 메서드, NumberOfSouls은 설정할 소울의 수
    FORCEINLINE void  SetSouls(int32 NumberOfSouls) { Souls = NumberOfSouls; }


};
