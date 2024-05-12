// Copyright Epic Games, Inc. All Rights Reserved.


#include "BloodyGroundGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "BloodyGround/Character/BaseCharacter.h"

ABloodyGroundGameModeBase::ABloodyGroundGameModeBase()
{
    // ABaseCharacter Ŭ������ DefaultPawnClass�� ����
    DefaultPawnClass = ABaseCharacter::StaticClass();
}

void ABloodyGroundGameModeBase::RespawnPlayer(APlayerController* PC)
{
    if (!PC || !HasAuthority()) // ���� ���� Ȯ��
    {
        return;
    }

    // PlayerStart ���͸� ��� ã��
    TArray<AActor*> PlayerStarts;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);

    // PlayerStart ���Ͱ� ������ ����
    if (PlayerStarts.Num() == 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("No PlayerStarts found on map."));
        return;
    }

    // ������ PlayerStart ����
    int32 Index = FMath::RandRange(0, PlayerStarts.Num() - 1);
    APlayerStart* ChosenStart = Cast<APlayerStart>(PlayerStarts[Index]);

    if (!ChosenStart)
    {
        UE_LOG(LogTemp, Warning, TEXT("Selected PlayerStart is not valid."));
        return;
    }

    // �� ĳ���� ���� ��ġ �� ȸ�� ����
    FVector SpawnLocation = ChosenStart->GetActorLocation();
    FRotator SpawnRotation = ChosenStart->GetActorRotation();

    // ĳ���� Ŭ������ ��ȿ���� Ȯ��
    if (!DefaultPawnClass)
    {
        UE_LOG(LogTemp, Warning, TEXT("DefaultPawnClass is not set."));
        return;
    }

    // �� ĳ���� ����
    FActorSpawnParameters SpawnParams;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    ABaseCharacter* NewCharacter = GetWorld()->SpawnActor<ABaseCharacter>(DefaultPawnClass, SpawnLocation, SpawnRotation, SpawnParams);

    if (NewCharacter)
    {
        // �� ĳ���͸� �÷��̾� ��Ʈ�ѷ��� �Ҵ�
        PC->Possess(NewCharacter);
    }
}
