// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"
#include "FantasyWorld/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void ASoul::Tick(float DeltaTime)
{
	// 부모 클래스의 Tick 함수를 호출합니다.
	Super::Tick(DeltaTime);

	// 이 액터의 Z축 위치를 가져옵니다.
	const double LocationZ = GetActorLocation().Z;

	// 이 액터의 Z 위치가 목표 Z 위치(DesiredZ)보다 높을 경우
	if (LocationZ > DesiredZ) {
		// DeltaLocation을 계산하여 Z축 방향으로 얼마나 이동할지 결정합니다.
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		// 계산된 DeltaLocation 만큼 액터를 이동시킵니다.
		AddActorWorldOffset(DeltaLocation);
	}
}

// BeginPlay 함수는 게임이 시작되거나 액터가 생성될 때 호출되는 함수입니다.
void ASoul::BeginPlay()
{
	// 부모 클래스의 BeginPlay 함수를 호출합니다.
	Super::BeginPlay();

	// 시작 위치와 끝 위치를 설정합니다.
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	// 오브젝트 타입을 설정합니다. (여기서는 ObjectTypeQuery1)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	// 무시할 액터를 설정합니다. (여기서는 이 액터의 소유자)
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	// 라인 트레이스 결과를 저장할 변수입니다.
	FHitResult HitResult;

	// 라인 트레이스를 실행합니다.
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	// 목표 Z 위치를 설정합니다. (트레이스가 맞은 지점의 Z 좌표 + 50)
	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

// 구형 컴포넌트와 다른 액터가 겹칠 때 호출되는 함수입니다.
void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 다른 액터가 IPickupInterface를 구현하고 있는지 확인합니다.
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	// 인터페이스를 구현한 경우
	if (PickupInterface) {
		// 인터페이스의 AddSouls 함수를 호출하여 이 객체를 전달합니다.
		PickupInterface->AddSouls(this);

		// Pickup 시스템과 소리를 실행합니다.
		SpawnPickupSystem();
		SpawnPickupSound();

		// 이 액터를 파괴합니다.
		Destroy();
	}
}
