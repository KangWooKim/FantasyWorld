#include "BaseZombieAnimInstance.h"
#include "BaseZombie.h"

void UBaseZombieAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    // �̵� �ӵ� ������Ʈ
    ABaseZombie* Zombie = Cast<ABaseZombie>(TryGetPawnOwner());
    if (Zombie)
    {
        MovementSpeed = Zombie->GetVelocity().Size();

        ZombieState = Zombie->GetZombieState();
    }
}
