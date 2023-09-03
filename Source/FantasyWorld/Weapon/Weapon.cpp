// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/BoxComponent.h"
#include "FantasyWorld/HitInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"

// AWeapon 클래스의 기본 생성자입니다.
AWeapon::AWeapon()
{
    // 무기의 메시(모델)을 생성하고 초기화합니다.
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
    // 무기 메시의 모든 채널에 대한 충돌 응답을 무시하도록 설정합니다.
    WeaponMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    // 무기 메시의 충돌을 비활성화합니다.
    WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 무기 메시를 이 객체의 루트 컴포넌트로 설정합니다.
    RootComponent = WeaponMesh;

    // 무기의 충돌 박스를 생성하고 초기화합니다.
    WeaponBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Weapon Box"));
    // 무기 박스를 루트 컴포넌트에 부착합니다.
    WeaponBox->SetupAttachment(GetRootComponent());
    // 무기 박스의 충돌을 비활성화합니다.
    WeaponBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // 무기 박스의 모든 채널에 대한 충돌 응답을 Overlap으로 설정합니다.
    WeaponBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
    // 무기 박스가 Pawn 채널과 충돌할 때의 응답을 무시하도록 설정합니다.
    WeaponBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

    // 박스 트레이스의 시작 지점을 설정합니다.
    BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace Start"));
    // 박스 트레이스 시작 지점을 루트 컴포넌트에 부착합니다.
    BoxTraceStart->SetupAttachment(GetRootComponent());

    // 박스 트레이스의 종료 지점을 설정합니다.
    BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("Box Trace End"));
    // 박스 트레이스 종료 지점을 루트 컴포넌트에 부착합니다.
    BoxTraceEnd->SetupAttachment(GetRootComponent());
}

// BeginPlay 메서드에서 호출되는 내용입니다.
void AWeapon::BeginPlay()
{
    // 부모 클래스의 BeginPlay를 호출합니다.
    Super::BeginPlay();
    // 무기 박스가 다른 객체와 겹칠 때 OnBoxOverlap 함수를 호출하도록 설정합니다.
    WeaponBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

// 무기를 장착하는 함수입니다.
// InParent: 무기를 부착할 부모 컴포넌트입니다.
// InSocketName: 부착될 소켓의 이름입니다.
// NewOwner: 무기의 새로운 주인입니다.
// NewInstigator: 이 무기를 사용하는 행위자입니다.
void AWeapon::Equip(USceneComponent* InParent, FName InSocketName, AActor* NewOwner, APawn* NewInstigator)
{
    // 무기의 주인을 설정합니다.
    SetOwner(NewOwner);
    // 무기의 행위자를 설정합니다.
    SetInstigator(NewInstigator);
    // 무기를 부착할 소켓과 부모 컴포넌트를 설정합니다.
    AttachMeshToSocket(InParent, InSocketName);
    // 무기를 장착할 때의 사운드를 재생합니다.
    PlayEquipSound();
}

// 무기를 장착할 때 사운드를 재생하는 함수입니다.
void AWeapon::PlayEquipSound()
{
    // EquipSound이 null이 아니면 사운드를 재생합니다.
    if (EquipSound)
    {
        UGameplayStatics::PlaySoundAtLocation(
            this,  // 사운드를 재생할 객체입니다.
            EquipSound,  // 재생할 사운드 애셋입니다.
            GetActorLocation()  // 사운드를 재생할 위치입니다.
        );
    }
}


// 무기의 메시를 부모 객체의 특정 소켓에 부착하는 함수입니다.
// InParent: 부모 컴포넌트입니다.
// InSocketName: 부착될 소켓의 이름입니다.
void AWeapon::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	// 부착 규칙을 설정합니다. SnapToTarget은 타겟에 바로 부착됩니다.
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	// 무기 메시를 부모 컴포넌트의 특정 소켓에 부착합니다.
	WeaponMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

// 무기의 박스와 다른 액터가 겹칠 때 호출되는 함수입니다.
void AWeapon::OnBoxOverlap(
	UPrimitiveComponent* OverlappedComponent, // 겹친 현재 객체의 컴포넌트입니다. 이 경우에는 무기의 박스 컴포넌트일 것입니다.
	AActor* OtherActor,                       // 겹친 다른 액터입니다. 예를 들어, 적 캐릭터가 될 수 있습니다.
	UPrimitiveComponent* OtherComp,            // 겹친 다른 액터의 컴포넌트입니다.
	int32 OtherBodyIndex,                      // 다중 컴포넌트가 있는 경우, 겹친 다른 액터의 몸체 인덱스를 나타냅니다.
	bool bFromSweep,                            // 스윕 테스트 결과인지 여부입니다. 스윕 테스트는 물체가 이동하는 경로에 대해 충돌을 검사합니다.
	const FHitResult& SweepResult)              // 스윕 테스트의 결과입니다. 이 정보는 충돌 위치, 충돌한 물체 등을 포함합니다.
{
	// 겹친 액터가 같은 타입이면 반환합니다.
	if (ActorIsSameType(OtherActor)) return;

	FHitResult BoxHit;
	// 박스 트레이스를 수행합니다.
	BoxTrace(BoxHit);

	// 트레이스가 어떤 액터에게도 맞지 않았다면, 함수를 종료합니다.
	if (!BoxHit.GetActor()) return;

	// 충돌한 액터가 같은 타입이거나 "Dead" 태그를 가지고 있다면 반환합니다.
	if (ActorIsSameType(BoxHit.GetActor()) || BoxHit.GetActor()->ActorHasTag(FName(TEXT("Dead")))) return;

	// 데미지를 적용합니다.
	UGameplayStatics::ApplyDamage(BoxHit.GetActor(), Damage, GetInstigator()->GetController(), this, UDamageType::StaticClass());

	// 데미지 적용 이후의 로직을 처리합니다.
	ExecuteGetHit(BoxHit);

	// 필드 생성 함수를 호출합니다. 이 필드는 충돌 위치에 생성됩니다.
	CreateFields(BoxHit.ImpactPoint);
}

// 현재 액터와 비교할 액터가 같은 타입인지 확인하는 함수입니다.
bool AWeapon::ActorIsSameType(AActor* OtherActor)
{
	return GetOwner()->ActorHasTag(TEXT("Enemy")) && OtherActor->ActorHasTag(TEXT("Enemy"));
}

// 데미지를 적용한 후 호출되는 함수입니다.
void AWeapon::ExecuteGetHit(FHitResult& BoxHit)
{
	// 충돌한 액터의 Hit 인터페이스를 확인합니다.
	IHitInterface* HitInterface = Cast<IHitInterface>(BoxHit.GetActor());

	// 인터페이스가 존재한다면, GetHit 함수를 호출합니다.
	if (HitInterface)
	{
		HitInterface->Execute_GetHit(BoxHit.GetActor(), BoxHit.ImpactPoint, GetOwner());
	}
}

// 박스 트레이스를 수행하는 함수입니다.
void AWeapon::BoxTrace(FHitResult& BoxHit)
{
	// 박스 트레이스의 시작과 끝 지점을 설정합니다.
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();

	// 트레이스에서 무시할 액터의 배열입니다.
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(GetOwner());

	// 추가로 무시할 액터를 배열에 넣습니다.
	for (AActor* Actor : IgnoreActors)
	{
		ActorsToIgnore.AddUnique(Actor);
	}

	// 실제로 박스 트레이스를 수행합니다.
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

	// 트레이스에서 충돌한 액터를 무시 액터 목록에 추가합니다.
	IgnoreActors.AddUnique(BoxHit.GetActor());
}
