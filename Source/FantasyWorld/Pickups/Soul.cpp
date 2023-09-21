// Fill out your copyright notice in the Description page of Project Settings.


#include "Soul.h"
#include "FantasyWorld/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/KismetSystemLibrary.h"


void ASoul::Tick(float DeltaTime)
{
	// �θ� Ŭ������ Tick �Լ��� ȣ���մϴ�.
	Super::Tick(DeltaTime);

	// �� ������ Z�� ��ġ�� �����ɴϴ�.
	const double LocationZ = GetActorLocation().Z;

	// �� ������ Z ��ġ�� ��ǥ Z ��ġ(DesiredZ)���� ���� ���
	if (LocationZ > DesiredZ) {
		// DeltaLocation�� ����Ͽ� Z�� �������� �󸶳� �̵����� �����մϴ�.
		const FVector DeltaLocation = FVector(0.f, 0.f, DriftRate * DeltaTime);
		// ���� DeltaLocation ��ŭ ���͸� �̵���ŵ�ϴ�.
		AddActorWorldOffset(DeltaLocation);
	}
}

// BeginPlay �Լ��� ������ ���۵ǰų� ���Ͱ� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
void ASoul::BeginPlay()
{
	// �θ� Ŭ������ BeginPlay �Լ��� ȣ���մϴ�.
	Super::BeginPlay();

	// ���� ��ġ�� �� ��ġ�� �����մϴ�.
	const FVector Start = GetActorLocation();
	const FVector End = Start - FVector(0.f, 0.f, 2000.f);

	// ������Ʈ Ÿ���� �����մϴ�. (���⼭�� ObjectTypeQuery1)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery1);

	// ������ ���͸� �����մϴ�. (���⼭�� �� ������ ������)
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetOwner());

	// ���� Ʈ���̽� ����� ������ �����Դϴ�.
	FHitResult HitResult;

	// ���� Ʈ���̽��� �����մϴ�.
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

	// ��ǥ Z ��ġ�� �����մϴ�. (Ʈ���̽��� ���� ������ Z ��ǥ + 50)
	DesiredZ = HitResult.ImpactPoint.Z + 50.f;
}

// ���� ������Ʈ�� �ٸ� ���Ͱ� ��ĥ �� ȣ��Ǵ� �Լ��Դϴ�.
void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// �ٸ� ���Ͱ� IPickupInterface�� �����ϰ� �ִ��� Ȯ���մϴ�.
	IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);

	// �������̽��� ������ ���
	if (PickupInterface) {
		// �������̽��� AddSouls �Լ��� ȣ���Ͽ� �� ��ü�� �����մϴ�.
		PickupInterface->AddSouls(this);

		// Pickup �ý��۰� �Ҹ��� �����մϴ�.
		SpawnPickupSystem();
		SpawnPickupSound();

		// �� ���͸� �ı��մϴ�.
		Destroy();
	}
}
