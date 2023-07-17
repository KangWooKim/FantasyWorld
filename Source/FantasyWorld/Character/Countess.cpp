// Fill out your copyright notice in the Description page of Project Settings.


#include "Countess.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "FantasyWorld/HitInterface.h"


ACountess::ACountess() {
	WeaponBoxFirst = CreateDefaultSubobject<UBoxComponent>(TEXT("First Blade"));
	WeaponBoxSecond = CreateDefaultSubobject<UBoxComponent>(TEXT("Second Blade"));
	WeaponBoxFirst->SetupAttachment(GetMesh(), TEXT("hand_l"));
	WeaponBoxSecond->SetupAttachment(GetMesh(), TEXT("hand_r"));

	WeaponBoxFirst->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxFirst->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxFirst->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	WeaponBoxSecond->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponBoxSecond->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	WeaponBoxSecond->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStartFirst = CreateDefaultSubobject<USceneComponent>(TEXT("First Box Trace Start"));
	BoxTraceStartFirst->SetupAttachment(GetMesh(), TEXT("hand_l"));

	BoxTraceEndFirst = CreateDefaultSubobject<USceneComponent>(TEXT("First Box Trace End"));
	BoxTraceEndFirst->SetupAttachment(GetMesh(), TEXT("hand_l"));

	BoxTraceStartSecond = CreateDefaultSubobject<USceneComponent>(TEXT("Second Box Trace Start"));
	BoxTraceStartSecond->SetupAttachment(GetMesh(), TEXT("hand_r"));

	BoxTraceEndSecond = CreateDefaultSubobject<USceneComponent>(TEXT("Second Box Trace End"));
	BoxTraceEndSecond->SetupAttachment(GetMesh(), TEXT("hand_r"));

}

void ACountess::BoxTrace(FHitResult& BoxHit)
{
	const FVector StartFirst = BoxTraceStartFirst->GetComponentLocation();
	const FVector EndFirst = BoxTraceStartFirst->GetComponentLocation();

	const FVector StartSecond = BoxTraceStartFirst->GetComponentLocation();
	const FVector EndSecond = BoxTraceStartFirst->GetComponentLocation();

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	ActorsToIgnore.Add(GetOwner());

	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartFirst,
		EndFirst,
		BoxTraceExtent,
		BoxTraceStartFirst->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		StartSecond,
		EndSecond,
		BoxTraceExtent,
		BoxTraceStartSecond->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		BoxHit,
		true
	);
	IgnoreActors.AddUnique(BoxHit.GetActor());

}

void ACountess::ExecuteGetHit(FHitResult& BoxHit)
{
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}