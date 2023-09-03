// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

// AWeapon Ŭ������ �⺻ �������Դϴ�.
AWeapon::AWeapon()
{
    // ������ �޽�(��)�� �����ϰ� �ʱ�ȭ�մϴ�.
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    // ���� �޽��� ��� ä�ο� ���� �浹 ������ �����ϵ��� �����մϴ�.
    WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    // ���� �޽��� �浹�� ��Ȱ��ȭ�մϴ�.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // ���� �޽ø� �� ��ü�� ��Ʈ ������Ʈ�� �����մϴ�.
    RootComponent = WeaponMesh;

    // ������ �浹 �ڽ��� �����ϰ� �ʱ�ȭ�մϴ�.
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    // ���� �ڽ��� ��Ʈ ������Ʈ�� �����մϴ�.
    WeaponBox->SetupAttachment(GetRootComponent());
    // ���� �ڽ��� �浹�� ��Ȱ��ȭ�մϴ�.
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // ���� �ڽ��� ��� ä�ο� ���� �浹 ������ Overlap���� �����մϴ�.
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    // ���� �ڽ��� Pawn ä�ΰ� �浹�� ���� ������ �����ϵ��� �����մϴ�.
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    // �ڽ� Ʈ���̽��� ���� ������ �����մϴ�.
    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    // �ڽ� Ʈ���̽� ���� ������ ��Ʈ ������Ʈ�� �����մϴ�.
    BoxTraceStart->SetupAttachment(GetRootComponent());

    // �ڽ� Ʈ���̽��� ���� ������ �����մϴ�.
    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    // �ڽ� Ʈ���̽� ���� ������ ��Ʈ ������Ʈ�� �����մϴ�.
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

// BeginPlay �޼��忡�� ȣ��Ǵ� �����Դϴ�.
void AWeapon::BeginPlay()
{
    // �θ� Ŭ������ BeginPlay�� ȣ���մϴ�.
    Super::BeginPlay();
    // ���� �ڽ��� �ٸ� ��ü�� ��ĥ �� OnBoxOverlap �Լ��� ȣ���ϵ��� �����մϴ�.
    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

// ���⸦ �����ϴ� �Լ��Դϴ�.
// InParent: ���⸦ ������ �θ� ������Ʈ�Դϴ�.
// InSocketName: ������ ������ �̸��Դϴ�.
// NewOwner: ������ ���ο� �����Դϴ�.
// NewInstigator: �� ���⸦ ����ϴ� �������Դϴ�.
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
    // ������ ������ �����մϴ�.
    SetOwner(NewOwner);
    // ������ �����ڸ� �����մϴ�.
    SetInstigator(NewInstigator);
    // ���⸦ ������ ���ϰ� �θ� ������Ʈ�� �����մϴ�.
    AttachMeshToSocket(InParent, InSocketName);
    // ���⸦ ������ ���� ���带 ����մϴ�.
    PlayEquipSound();
}

// ���⸦ ������ �� ���带 ����ϴ� �Լ��Դϴ�.
void AWeapon::PlayEquipSound()
{
    // EquipSound�� null�� �ƴϸ� ���带 ����մϴ�.
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,  // ���带 ����� ��ü�Դϴ�.
            EquipSound,  // ����� ���� �ּ��Դϴ�.
            GetActorLocation()  // ���带 ����� ��ġ�Դϴ�.
        );
    }
}


// ������ �޽ø� �θ� ��ü�� Ư�� ���Ͽ� �����ϴ� �Լ��Դϴ�.
// InParent: �θ� ������Ʈ�Դϴ�.
// InSocketName: ������ ������ �̸��Դϴ�.
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	// ���� ��Ģ�� �����մϴ�. SnapToTarget�� Ÿ�ٿ� �ٷ� �����˴ϴ�.
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	// ���� �޽ø� �θ� ������Ʈ�� Ư�� ���Ͽ� �����մϴ�.
	WeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

// ������ �ڽ��� �ٸ� ���Ͱ� ��ĥ �� ȣ��Ǵ� �Լ��Դϴ�.
void AWeapon::OnBoxOverlap(
	UPrimitiveComponent* OverlappedComponent, // ��ģ ���� ��ü�� ������Ʈ�Դϴ�. �� ��쿡�� ������ �ڽ� ������Ʈ�� ���Դϴ�.
	AActor* OtherActor,                       // ��ģ �ٸ� �����Դϴ�. ���� ���, �� ĳ���Ͱ� �� �� �ֽ��ϴ�.
	UPrimitiveComponent* OtherComp,            // ��ģ �ٸ� ������ ������Ʈ�Դϴ�.
	int32 OtherBodyIndex,                      // ���� ������Ʈ�� �ִ� ���, ��ģ �ٸ� ������ ��ü �ε����� ��Ÿ���ϴ�.
	bool bFromSweep,                            // ���� �׽�Ʈ ������� �����Դϴ�. ���� �׽�Ʈ�� ��ü�� �̵��ϴ� ��ο� ���� �浹�� �˻��մϴ�.
	const FHitResult& SweepResult)              // ���� �׽�Ʈ�� ����Դϴ�. �� ������ �浹 ��ġ, �浹�� ��ü ���� �����մϴ�.
{
	// ��ģ ���Ͱ� ���� Ÿ���̸� ��ȯ�մϴ�.
	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	// �ڽ� Ʈ���̽��� �����մϴ�.
	BoxTrace(BoxHit);

	// Ʈ���̽��� � ���Ϳ��Ե� ���� �ʾҴٸ�, �Լ��� �����մϴ�.
	if (!BoxHit.GetActor()) return;

	// �浹�� ���Ͱ� ���� Ÿ���̰ų� "Dead" �±׸� ������ �ִٸ� ��ȯ�մϴ�.
	if (ActorIsSameType(BoxHit.GetActor()) || BoxHit.GetActor()->ActorHasTag(FName(TEXT("Dead")))) return;

	// �������� �����մϴ�.
	UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

	// ������ ���� ������ ������ ó���մϴ�.
	ExecuteGetHit(BoxHit);

	// �ʵ� ���� �Լ��� ȣ���մϴ�. �� �ʵ�� �浹 ��ġ�� �����˴ϴ�.
	CreateFields(BoxHit.ImpactPoint);
}

// ���� ���Ϳ� ���� ���Ͱ� ���� Ÿ������ Ȯ���ϴ� �Լ��Դϴ�.
bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

// �������� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	// �浹�� ������ Hit �������̽��� Ȯ���մϴ�.
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());

	// �������̽��� �����Ѵٸ�, GetHit �Լ��� ȣ���մϴ�.
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

// �ڽ� Ʈ���̽��� �����ϴ� �Լ��Դϴ�.
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	// �ڽ� Ʈ���̽��� ���۰� �� ������ �����մϴ�.
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	// Ʈ���̽����� ������ ������ �迭�Դϴ�.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	// �߰��� ������ ���͸� �迭�� �ֽ��ϴ�.
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	// ������ �ڽ� Ʈ���̽��� �����մϴ�.
	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		BoxTraceExtent,
		BoxTraceStart->GetComponentRotation(),
		ETraceTypeQuery::TraceTypeQuery1,
		false,
		ActorsToIgnore,
		bShowBoxDebug ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxHit,
		true
	);

	// Ʈ���̽����� �浹�� ���͸� ���� ���� ��Ͽ� �߰��մϴ�.
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
