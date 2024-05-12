#include "KeyItem.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "BloodyGround/Character/BaseCharacter.h"

AKeyItem::AKeyItem()
{
    PrimaryActorTick.bCanEverTick = true;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    // ��Ʈ��ũ ���� Ȱ��ȭ
    SetReplicates(true);
    SetReplicateMovement(true);

    // �浹 ���� ��� �߰� �� ����
    USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    SphereComponent->SetSphereRadius(100.0f);
    SphereComponent->SetupAttachment(RootComponent);
    SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AKeyItem::OnOverlapBegin);
}

void AKeyItem::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
}

void AKeyItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // ���Ʒ� ������ ����
    FVector NewLocation = StartLocation;
    RunningTime += DeltaTime;
    float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
    NewLocation.Z += DeltaHeight * 20.0f; // ���� ������ �Ű�����
    SetActorLocation(NewLocation);
}

void AKeyItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (HasAuthority()) // ���������� ����
    {
        ABaseCharacter* BaseCharacter = Cast<ABaseCharacter>(OtherActor);
        if (BaseCharacter && OtherActor != this && OtherComp)
        {
            // �������� ȿ���� ��� �� ������ ���� ó��
            Multicast_PlaySoundAndDestroy();
        }
    }
}

void AKeyItem::Multicast_PlaySoundAndDestroy_Implementation()
{
    // ��� Ŭ���̾�Ʈ�� ȿ���� ���
    if (PickupSound)
    {
        UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
    }

    // ������ ����
    Destroy();
}