#include "BaseWeapon.h"
#include "Animation/AnimationAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "BloodyGround/Character/BaseCharacter.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "BloodyGround/Component/InventoryComponent.h"
#include "BloodyGround/PlayerController/BloodyGroundPlayerController.h"
#include "BloodyGround/Enemy/EliteZombie.h"
#include "BloodyGround/Component/ServerLocationComponent.h"


ABaseWeapon::ABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

    // ���� ���� ���� ����. �� ���ʹ� ���� �����մϴ�.
    bReplicates = true;

    // ������ ���� ���� ����. �� ������ �������� �����˴ϴ�.
    SetReplicateMovement(true);
    SetReplicates(true);

    // ���̷�Ż �޽� ������Ʈ �ʱ�ȭ 
    
    SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
    SetRootComponent(SkeletalMeshComponent);

    // ���� �޽��� �浹 ���� ����
    SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
    SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
    SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    NoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));

    HitThreshold = 100.f;
}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

    WeaponState = EWeaponState::None;

    Character = Cast<ABaseCharacter>(GetOwner());
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ABaseWeapon, WeaponState);
    DOREPLIFETIME(ABaseWeapon, CurrentAmmo);
}

void ABaseWeapon::ChangeWeapon()
{
}

void ABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseWeapon::Fire()
{
    if (Character->HasAuthority())
    {
        MulticastPlayFireAnimation();
        WeaponNoise(1.f);
    }
    else if(Character->IsLocallyControlled())
    {
        ServerPlayFireAnimation();
    }

    WeaponState = EWeaponState::Fire;

    if (Character)
    {
        FVector StartLocation = SkeletalMeshComponent->GetSocketLocation(TEXT("MuzzleFlash"));
        FVector ForwardVector = Character->GetControlRotation().Vector();
        FVector EndLocation = StartLocation + ForwardVector * 10000.0f;

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        QueryParams.AddIgnoredActor(this);
        QueryParams.AddIgnoredActor(Character);

        if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, QueryParams))
        {
            ABaseZombie* HitZombie = Cast<ABaseZombie>(HitResult.GetActor());
            if (HitZombie)
            {
                if (Character->HasAuthority())
                {
                    FLocationTimeData TempLocationData = HitZombie->GetServerLocationComponent()->GetLocationData();
                    FHitResultData HitResultData = HitZombie->GetServerLocationComponent()->ServerTrace(StartLocation, EndLocation, TempLocationData);
                    if (HitResultData.bHitBody)
                    {
                        HitZombie->TakeShot(HitResultData, Damage);
                        UGameplayStatics::ApplyDamage(HitZombie, Damage, Character->GetController(), this, UDamageType::StaticClass());
                    }
                }
                else if (Character->IsLocallyControlled())
                {
                    // Ŭ���̾�Ʈ���� ���� ���θ� ������ ����
                    ServerCheckHit(HitResult, GetWorld()->GetTimeSeconds(), StartLocation, ForwardVector);
                }
            }
        }
    }
}

    void ABaseWeapon::ServerCheckHit_Implementation(FHitResult ClientHitResult, float HitTime, FVector StartLocation, FVector EndDirection)
    {
        ABaseZombie* HitZombie = Cast<ABaseZombie>(ClientHitResult.GetActor());
        if (!HitZombie) return;

        ABloodyGroundPlayerController* PlayerController = Cast<ABloodyGroundPlayerController>(Character->GetController());
        if (!PlayerController) return;

        float RTT = PlayerController->GetRoundTripTime();
        UServerLocationComponent* ServerLocationComp = HitZombie->FindComponentByClass<UServerLocationComponent>();
        if (!ServerLocationComp) return;

        float CorrectedTime = HitTime - (RTT / 2.0f);
        FLocationTimeData LocationData = ServerLocationComp->GetInterpolatedLocationData(CorrectedTime);
        FVector EndLocation = StartLocation + EndDirection * 10000.0f;

        FHitResultData HitResults = ServerLocationComp->CheckHitWithTrace(StartLocation, EndLocation, LocationData);

        // ���뿡 ������ ���
        if (HitResults.bHitBody)
        {
            HitZombie->TakeShot(HitResults, Damage);
            UGameplayStatics::ApplyDamage(HitZombie, Damage, Character->GetController(), this, UDamageType::StaticClass());
        }
    }

bool ABaseWeapon::ServerCheckHit_Validate(FHitResult HitResult, float HitTime, FVector StartLocation, FVector EndDirection)
{
    return true; // �߰����� ��ȿ�� �˻� ������ �ʿ��� �� �ֽ��ϴ�
}

void ABaseWeapon::WeaponNoise_Implementation(float Loudness)
{
    if (Character)
    {
        Character->MakeNoise(Loudness, Character, GetActorLocation());
    }
}

bool ABaseWeapon::CanFire()
{
    return true;
}

void ABaseWeapon::FireEnd()
{
    WeaponState = EWeaponState::None;
}

void ABaseWeapon::ReloadEnd()
{
    WeaponState = EWeaponState::None;
}

void ABaseWeapon::Reload()
{
    if (Character)
    {
        // Ŭ���̾�Ʈ���� ������ ������ ��û ����
        if (Character->IsLocallyControlled() && !Character->HasAuthority())
        {
            ServerReload();
        }

        // �������� ���� ������ ó��
        else if (Character->HasAuthority())
        {
            PerformReload();
        }
    }
}

void ABaseWeapon::PerformReload()
{
    if (WeaponState == EWeaponState::None)
    {
        if (Character == nullptr || Character->InventoryComp == nullptr)
        {
            return; // ĳ���� �Ǵ� �κ��丮 ������Ʈ�� ���� ��� ����
        }

        WeaponState = EWeaponState::Reload;

        // ��� Ŭ���̾�Ʈ���� ������ �ִϸ��̼� ���
        MulticastPlayReloadAnimation();

        // ���� �߻�
        WeaponNoise(0.3f);  // ������ ���� ũ�� ����
    }
}


void ABaseWeapon::ServerReload_Implementation()
{
    PerformReload();
}

void ABaseWeapon::MulticastPlayReloadAnimation_Implementation()
{
    if (ReloadAnimation)
    {
        SkeletalMeshComponent->PlayAnimation(ReloadAnimation, false);
    }
}

EWeaponType ABaseWeapon::GetCurrentWeaponType()
{
	return EWeaponType::None;
}

void ABaseWeapon::ServerPlayFireAnimation_Implementation()
{
    MulticastPlayFireAnimation();
    WeaponNoise(1.f);
}

void ABaseWeapon::MulticastPlayFireAnimation_Implementation()
{
    if (FireAnimation)
    {
        SkeletalMeshComponent->PlayAnimation(FireAnimation, false);
    }
}