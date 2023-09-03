// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "FantasyWorld/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
// 기본 생성자
AItem::AItem()
{
    // 매 프레임마다 Tick() 함수를 호출할 것인지 설정. 성능을 개선하기 위해 필요하지 않으면 끌 수 있음.
    PrimaryActorTick.bCanEverTick = true;

    // 아이템 메쉬 초기화
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    // 모든 채널에 대한 충돌 응답을 무시
    ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    // 충돌을 비활성화
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 루트 컴포넌트 설정
    RootComponent = ItemMesh;

    // 구 형태의 컴포넌트 초기화
    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    // 루트 컴포넌트에 부착
    Sphere->SetupAttachment(GetRootComponent());

    // 이펙트 컴포넌트 초기화
    ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
    // 루트 컴포넌트에 부착
    ItemEffect->SetupAttachment(GetRootComponent());
}

// 게임 시작이나 배치될 때 호출
void AItem::BeginPlay()
{
    Super::BeginPlay();

    // 오버랩 이벤트 등록
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
    // 오버랩 종료 이벤트 등록
    Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

// 사인 함수 변형
float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

// 코사인 함수 변형
float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// 구 오버랩 이벤트 처리
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 오버랩된 액터가 IPickupInterface를 구현하고 있는지 확인
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
    if (PickupInterface) {
        // 오버랩 중인 아이템을 설정
        PickupInterface->SetOverlappingItem(this);
    }
}

// 구 오버랩 종료 이벤트 처리
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // 오버랩 종료된 액터가 IPickupInterface를 구현하고 있는지 확인
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
    if (PickupInterface) {
        // 오버랩 중인 아이템을 null로 설정
        PickupInterface->SetOverlappingItem(nullptr);
    }
}

// 아이템 획득 이펙트 생성
void AItem::SpawnPickupSystem()
{
    if (PickupEffect) {
        // 이펙트 스폰
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this,
            PickupEffect,
            GetActorLocation()
        );
    }
}

// 아이템 획득 사운드 생성
void AItem::SpawnPickupSound()
{
    if (PickupSound) {
        // 사운드 스폰
        UGameplayStatics::SpawnSoundAtLocation(
            this,
            PickupSound,
            GetActorLocation()
        );
    }
}

// 매 프레임마다 호출되는 함수
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 현재 시간 업데이트
    RunningTime += DeltaTime;
    // 아이템 상태가 Hovering인 경우
    if (ItemState == EItemState::EIS_Hovering) {
        // 아이템을 움직여 Hovering 효과 적용
        AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
    }
}
