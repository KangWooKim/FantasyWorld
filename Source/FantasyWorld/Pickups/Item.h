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
	// 클래스의 기본 속성을 설정하는 생성자
	AItem();

protected:
	// 게임 시작 또는 객체가 스폰될 때 호출되는 함수
	virtual void BeginPlay() override;

	// 사인 파동의 진폭을 설정하는 속성 (Blueprint에서 수정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float Amplitude = 0.25f;

	// 사인 파동의 시간 상수를 설정하는 속성 (Blueprint에서 수정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Parameters")
		float TimeConstant = 5.f;

	// 변환된 사인 값을 반환하는 함수
	UFUNCTION(BlueprintPure)
		float TransformedSin();

	// 변환된 코사인 값을 반환하는 함수
	UFUNCTION(BlueprintPure)
		float TransformedCos();

	// 두 값의 평균을 계산하는 템플릿 함수
	template<typename T>
	T Avg(T First, T Second);

	// 스피어 컴포넌트와 다른 액터가 겹쳤을 때 호출되는 함수
	// OverlappedComponent: 겹친 컴포넌트
	// OtherActor: 겹친 다른 액터
	// OtherComp: 겹친 다른 컴포넌트
	// OtherBodyIndex: 겹친 다른 컴포넌트의 바디 인덱스
	// bFromSweep: Sweep 연산으로 인한 겹침 여부
	// SweepResult: Sweep 연산 결과
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 스피어 컴포넌트와 다른 액터가 겹침이 끝났을 때 호출되는 함수
	// OverlappedComponent: 겹친 컴포넌트
	// OtherActor: 겹친 다른 액터
	// OtherComp: 겹친 다른 컴포넌트
	// OtherBodyIndex: 겹친 다른 컴포넌트의 바디 인덱스
	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 아이템을 먹을 때 효과를 생성하는 함수
	virtual void SpawnPickupSystem();

	// 아이템을 먹을 때 소리를 생성하는 함수
	virtual void SpawnPickupSound();

	// 아이템의 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* ItemMesh;

	// 아이템의 상태 (예: 호버링 중)
	EItemState ItemState = EItemState::EIS_Hovering;

	// 아이템의 충돌 범위를 나타내는 스피어 컴포넌트
	UPROPERTY(VisibleAnywhere)
		USphereComponent* Sphere;

	// 아이템의 특수 효과 (파티클)
	UPROPERTY(EditAnywhere)
		UNiagaraComponent* ItemEffect;

	// 아이템을 먹었을 때의 소리
	UPROPERTY(EditAnywhere)
		USoundBase* PickupSound;


public:
	// 매 프레임마다 호출되는 함수
	virtual void Tick(float DeltaTime) override;

private:
	// 내부적으로 사용되는, 실행 시간을 추적하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		float RunningTime;

	// 아이템을 먹었을 때의 파티클 효과
	UPROPERTY(EditAnywhere)
		class UNiagaraSystem* PickupEffect;


};

template<typename T>
inline T AItem::Avg(T First, T Second)
{
	return (First + Second) / 2;
}
