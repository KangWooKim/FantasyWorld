// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "FantasyWorld/PickupInterface.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
// �⺻ ������
AItem::AItem()
{
    // �� �����Ӹ��� Tick() �Լ��� ȣ���� ������ ����. ������ �����ϱ� ���� �ʿ����� ������ �� �� ����.
    PrimaryActorTick.bCanEverTick = true;

    // ������ �޽� �ʱ�ȭ
    ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    // ��� ä�ο� ���� �浹 ������ ����
    ItemMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    // �浹�� ��Ȱ��ȭ
    ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // ��Ʈ ������Ʈ ����
    RootComponent = ItemMesh;

    // �� ������ ������Ʈ �ʱ�ȭ
    Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
    // ��Ʈ ������Ʈ�� ����
    Sphere->SetupAttachment(GetRootComponent());

    // ����Ʈ ������Ʈ �ʱ�ȭ
    ItemEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Embers"));
    // ��Ʈ ������Ʈ�� ����
    ItemEffect->SetupAttachment(GetRootComponent());
}

// ���� �����̳� ��ġ�� �� ȣ��
void AItem::BeginPlay()
{
    Super::BeginPlay();

    // ������ �̺�Ʈ ���
    Sphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
    // ������ ���� �̺�Ʈ ���
    Sphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}

// ���� �Լ� ����
float AItem::TransformedSin()
{
    return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

// �ڻ��� �Լ� ����
float AItem::TransformedCos()
{
    return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

// �� ������ �̺�Ʈ ó��
void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // �������� ���Ͱ� IPickupInterface�� �����ϰ� �ִ��� Ȯ��
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
    if (PickupInterface) {
        // ������ ���� �������� ����
        PickupInterface->SetOverlappingItem(this);
    }
}

// �� ������ ���� �̺�Ʈ ó��
void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    // ������ ����� ���Ͱ� IPickupInterface�� �����ϰ� �ִ��� Ȯ��
    IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
    if (PickupInterface) {
        // ������ ���� �������� null�� ����
        PickupInterface->SetOverlappingItem(nullptr);
    }
}

// ������ ȹ�� ����Ʈ ����
void AItem::SpawnPickupSystem()
{
    if (PickupEffect) {
        // ����Ʈ ����
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(
            this,
            PickupEffect,
            GetActorLocation()
        );
    }
}

// ������ ȹ�� ���� ����
void AItem::SpawnPickupSound()
{
    if (PickupSound) {
        // ���� ����
        UGameplayStatics::SpawnSoundAtLocation(
            this,
            PickupSound,
            GetActorLocation()
        );
    }
}

// �� �����Ӹ��� ȣ��Ǵ� �Լ�
void AItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���� �ð� ������Ʈ
    RunningTime += DeltaTime;
    // ������ ���°� Hovering�� ���
    if (ItemState == EItemState::EIS_Hovering) {
        // �������� ������ Hovering ȿ�� ����
        AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
    }
}
