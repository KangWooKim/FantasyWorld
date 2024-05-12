#include "ServerLocationComponent.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"

UServerLocationComponent::UServerLocationComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    TimeToKeepData = 10.0f; // ��: 10�ʵ��ȵ���������
    RecordInterval = 0.1f; // ��: 0.1�ʸ�����ġ���
    LastRecordTime = 0.f;

    // ���� ������Ʈ �ʱ�ȭ
    CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Body"));
    LeftLegComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftLeg"));
    RightLegComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightLeg"));

    // ���� �Ǵܿ� ������Ʈ �ʱ�ȭ
    BodyHitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyHit"));
    LeftLegHitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftLegHit"));
    RightLegHitCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightLegHit"));

    HitCapsules = { BodyHitCapsule, LeftLegHitCapsule, RightLegHitCapsule, CapsuleComponent, LeftLegComponent, RightLegComponent};
}

void UServerLocationComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UServerLocationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (GetOwner()->GetLocalRole() == ROLE_Authority) // ���������� ����
    {
        if (GetWorld()->TimeSince(LastRecordTime) >= RecordInterval)
        {
            RecordLocation();
            CleanupOldData();
        }
    }
}

void UServerLocationComponent::RecordLocation()
{
    FVector CapsuleLocation = CapsuleComponent->GetComponentLocation();
    FRotator CapsuleRotation = CapsuleComponent->GetComponentRotation();

    FVector LeftLegLocation = LeftLegComponent->GetComponentLocation();
    FRotator LeftLegRotation = LeftLegComponent->GetComponentRotation();

    FVector RightLegLocation = RightLegComponent->GetComponentLocation();
    FRotator RightLegRotation = RightLegComponent->GetComponentRotation();

    float CurrentTime = GetWorld()->GetTimeSeconds();
    LocationHistory.Add(FLocationTimeData(
        FComponentLocationData(CapsuleLocation, CapsuleRotation),
        FComponentLocationData(LeftLegLocation, LeftLegRotation),
        FComponentLocationData(RightLegLocation, RightLegRotation),
        CurrentTime
    ));
    LastRecordTime = CurrentTime;
}

void UServerLocationComponent::CleanupOldData()
{
    float CurrentTime = GetWorld()->GetTimeSeconds();
    LocationHistory.RemoveAll([CurrentTime, this](const FLocationTimeData& Data) {
        return CurrentTime - Data.TimeStamp > TimeToKeepData;
        });
}

FLocationTimeData UServerLocationComponent::GetInterpolatedLocationData(float Time)
{
    FLocationTimeData PreviousData;
    FLocationTimeData NextData;

    for (int32 i = 0; i < LocationHistory.Num() - 1; ++i)
    {
        if (Time >= LocationHistory[i].TimeStamp && Time < LocationHistory[i + 1].TimeStamp)
        {
            PreviousData = LocationHistory[i];
            NextData = LocationHistory[i + 1];
            break;
        }
    }

    if (PreviousData.TimeStamp == 0.f && NextData.TimeStamp == 0.f)
    {
        // �ð��� �ش��ϴ� �����Ͱ� ����
        return FLocationTimeData();
    }
    else if (NextData.TimeStamp == 0.f)
    {
        // ��Ȯ�� ��ġ�ϴ� ������ ��ȯ
        return PreviousData;
    }
    else
    {
        // �����Ͽ� ��ȯ
        float Alpha = (Time - PreviousData.TimeStamp) / (NextData.TimeStamp - PreviousData.TimeStamp);
        FLocationTimeData InterpolatedData;
        InterpolatedData.CapsuleData.Location = FMath::Lerp(PreviousData.CapsuleData.Location, NextData.CapsuleData.Location, Alpha);
        InterpolatedData.CapsuleData.Rotation = FMath::Lerp(PreviousData.CapsuleData.Rotation, NextData.CapsuleData.Rotation, Alpha);
        InterpolatedData.LeftLegData.Location = FMath::Lerp(PreviousData.LeftLegData.Location, NextData.LeftLegData.Location, Alpha);
        InterpolatedData.LeftLegData.Rotation = FMath::Lerp(PreviousData.LeftLegData.Rotation, NextData.LeftLegData.Rotation, Alpha);
        InterpolatedData.RightLegData.Location = FMath::Lerp(PreviousData.RightLegData.Location, NextData.RightLegData.Location, Alpha);
        InterpolatedData.RightLegData.Rotation = FMath::Lerp(PreviousData.RightLegData.Rotation, NextData.RightLegData.Rotation, Alpha);
        return InterpolatedData;
    }
}

FHitResultData  UServerLocationComponent::CheckHitWithTrace(const FVector& StartTrace, const FVector& EndTrace, const FLocationTimeData& LocationData)
{
    // ĸ�� ������Ʈ ���� �� ������Ʈ
    UpdateHitCapsule(BodyHitCapsule, LocationData.CapsuleData);

    // Body�� ���� Ʈ���̽� ����
    bool bHitBody = PerformLineTrace(StartTrace, EndTrace, BodyHitCapsule);

    // ĸ�� ������Ʈ �浹 ���� ����
    RestoreCollisionSettings();

    UpdateHitCapsule(LeftLegHitCapsule, LocationData.LeftLegData);
    UpdateHitCapsule(RightLegHitCapsule, LocationData.RightLegData);

    // Legs�� ���� Ʈ���̽� ���� (Body ����)
    bool bHitLeg = PerformLineTrace(StartTrace, EndTrace, LeftLegHitCapsule) || PerformLineTrace(StartTrace, EndTrace, RightLegHitCapsule);

    // ĸ�� ������Ʈ �浹 ���� ����
    RestoreCollisionSettings();

    return FHitResultData(bHitBody, bHitLeg);
}

FHitResultData  UServerLocationComponent::ServerTrace(const FVector& StartTrace, const FVector& EndTrace, const FLocationTimeData& LocationData)
{
    // ĸ�� ������Ʈ ���� �� ������Ʈ
    UpdateHitCapsule(CapsuleComponent, LocationData.CapsuleData);

    // Body�� ���� Ʈ���̽� ����
    bool bHitBody = PerformLineTrace(StartTrace, EndTrace, CapsuleComponent);

    // ĸ�� ������Ʈ �浹 ���� ����
    RestoreCollisionSettings();

    UpdateHitCapsule(LeftLegComponent, LocationData.LeftLegData);
    UpdateHitCapsule(RightLegComponent, LocationData.RightLegData);

    // Legs�� ���� Ʈ���̽� ����(Body ����)
    bool bHitLeg = PerformLineTrace(StartTrace, EndTrace, LeftLegComponent) || PerformLineTrace(StartTrace, EndTrace, RightLegComponent);

    // ĸ�� ������Ʈ �浹 ���� ����
    RestoreCollisionSettings();

    return FHitResultData(bHitBody, bHitLeg);
}

FLocationTimeData UServerLocationComponent::GetLocationData()
{
    FVector CapsuleLocation = CapsuleComponent->GetComponentLocation();
    FRotator CapsuleRotation = CapsuleComponent->GetComponentRotation();

    FVector LeftLegLocation = LeftLegComponent->GetComponentLocation();
    FRotator LeftLegRotation = LeftLegComponent->GetComponentRotation();

    FVector RightLegLocation = RightLegComponent->GetComponentLocation();
    FRotator RightLegRotation = RightLegComponent->GetComponentRotation();

    float CurrentTime = GetWorld()->GetTimeSeconds();

    return FLocationTimeData(
        FComponentLocationData(CapsuleLocation, CapsuleRotation),
        FComponentLocationData(LeftLegLocation, LeftLegRotation),
        FComponentLocationData(RightLegLocation, RightLegRotation),
        CurrentTime
    );
}

void UServerLocationComponent::UpdateHitCapsule(UCapsuleComponent* HitCapsule, const FComponentLocationData& LocationData)
{
    HitCapsule->SetWorldLocation(LocationData.Location);
    HitCapsule->SetWorldRotation(LocationData.Rotation);
    // ��ϵ� ���� ĸ�� ũ��� ����
    HitCapsule->SetCapsuleSize(CapsuleComponent->GetUnscaledCapsuleRadius(), CapsuleComponent->GetUnscaledCapsuleHalfHeight());
    HitCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    HitCapsule->SetCollisionResponseToChannel(ECC_ComponentCollision, ECR_Block);
}

bool UServerLocationComponent::PerformLineTrace(const FVector& StartTrace, const FVector& EndTrace, UCapsuleComponent* PrimaryCapsule)
{
    FHitResult HitResult;
    FCollisionQueryParams QueryParams;

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartTrace, EndTrace, ECC_ComponentCollision, QueryParams);
    return bHit && HitResult.GetComponent() == PrimaryCapsule;
}

void UServerLocationComponent::RestoreCollisionSettings()
{
    for (UCapsuleComponent* Capsule : HitCapsules)
    {
        Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        Capsule->SetCollisionResponseToChannel(ECC_ComponentCollision, ECR_Ignore);
    }
}