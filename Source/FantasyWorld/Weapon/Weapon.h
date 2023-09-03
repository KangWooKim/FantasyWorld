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
	
		// 헤더 파일에서 AWeapon 클래스 정의 부분

public:
	AWeapon();  // 기본 생성자

	// 무기를 장착하는 함수
	// InParent: 무기가 부착될 부모 컴포넌트
	// InSocketName: 부착될 소켓의 이름
	// NewOwner: 무기의 새로운 주인
	// NewInstigator: 무기를 사용하는 캐릭터
	void Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator);

	void PlayEquipSound();  // 무기를 장착할 때의 사운드를 재생하는 함수

	// 무기 메시를 특정 소켓에 부착하는 함수
	// InParent: 무기가 부착될 부모 컴포넌트
	// InSocketName: 부착될 소켓의 이름
	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	// 무시할 액터 목록
	TArray<AActor*> IgnoreActors;

protected:
	virtual void BeginPlay() override;  // 게임 시작 시 호출되는 함수

	// 오버랩 발생 시 호출되는 함수
	// OverlappedComponent: 오버랩이 발생한 컴포넌트
	// OtherActor: 오버랩이 발생한 다른 액터
	// OtherComp: 오버랩이 발생한 다른 컴포넌트
	// OtherBodyIndex: 오버랩된 컴포넌트의 인덱스
	// bFromSweep: Sweep 결과로 오버랩이 발생했는지 여부
	// SweepResult: 오버랩 결과
	UFUNCTION()
		void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 액터의 타입이 같은지 확인하는 함수
	// OtherActor: 확인할 다른 액터
	bool ActorIsSameType(AActor* OtherActor);

	// 히트 처리 로직을 실행하는 함수
	// BoxHit: 히트 결과 정보
	void ExecuteGetHit(FHitResult& BoxHit);

	// 블루프린트에서 구현 가능한 이벤트
	UFUNCTION(BlueprintImplementableEvent)
		void CreateFields(const FVector& FieldLocation);

	// 무기의 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* WeaponMesh;

private:
	// 상자 추적을 수행하는 함수
	// BoxHit: 추적 결과 정보
	void BoxTrace(FHitResult& BoxHit);

	// 상자 추적 범위
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		FVector BoxTraceExtent = FVector(5.f);

	// 디버그 표시 여부
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		bool bShowBoxDebug = false;

	// 장착 사운드
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		USoundBase* EquipSound;

	// 무기의 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
		UBoxComponent* WeaponBox;

	// 상자 추적 시작 위치
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceStart;

	// 상자 추적 종료 위치
	UPROPERTY(VisibleAnywhere)
		USceneComponent* BoxTraceEnd;

	// 무기의 데미지
	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
		float Damage = 20.f;

public:
	// 무기 박스 컴포넌트를 반환하는 인라인 함수
	FORCEINLINE UBoxComponent* GetWeaponBox() const { return WeaponBox; }

	// 무기 데미지를 설정하는 인라인 함수
	FORCEINLINE void SetWeaponDamage(float WeaponDamage) { Damage = WeaponDamage; }


};
