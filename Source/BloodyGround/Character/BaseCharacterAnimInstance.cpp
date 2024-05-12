#include "BaseCharacterAnimInstance.h"
#include "BaseCharacter.h"
#include "BloodyGround/Component/InventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UBaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ABaseCharacter* BaseChar = Cast<ABaseCharacter>(TryGetPawnOwner());
    if (BaseChar)
    {
        FVector Velocity = BaseChar->GetVelocity();
        GroundSpeed = Velocity.Size();

        // ĳ������ ����� ���� ����
        FVector ForwardVector = BaseChar->GetActorForwardVector();
        FVector RightVector = BaseChar->GetActorRightVector();

        CharacterState = BaseChar->GetCharacterState();

        // ���� �� �¿� �ӵ� ���
        float ForwardSpeed = FVector::DotProduct(Velocity, ForwardVector);
        float RightSpeed = FVector::DotProduct(Velocity, RightVector);

        // �ִϸ��̼� �������Ʈ ���� ������Ʈ
        bIsInAir = BaseChar->GetCharacterMovement()->IsFalling();
        bIsAiming = BaseChar->BattleComp ? BaseChar->BattleComp->bIsAiming : false;
        ForwardMovement = ForwardSpeed;
        RightMovement = RightSpeed;

        ABaseWeapon* CurrentWeapon = BaseChar->InventoryComp->GetCurrentWeapon();
        if (CurrentWeapon)
        {
            CurrentWeaponType = CurrentWeapon->GetCurrentWeaponType();
            LeftHandSocketLocation = CurrentWeapon->GetWeaponMesh()->GetSocketLocation(TEXT("LeftHandSocket"));
        }

        
    }
}

void UBaseCharacterAnimInstance::PlayWeaponSwitchAnimation()
{
    // ���� ���� �ִϸ��̼� ���� �� ��� ����
    // ��: Pistol -> MachineGun ���� ��, Pistol�� ����� MachineGun �̱� �ִϸ��̼� ���
}
