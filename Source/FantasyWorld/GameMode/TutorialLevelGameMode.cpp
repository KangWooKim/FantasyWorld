// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

// �⺻ ������, Ʃ�丮�� ���� ��� �ʱ�ȭ
ATutorialLevelGameMode::ATutorialLevelGameMode()
{
}

// BeginPlay �޼���, ���� ���۽� ����Ǵ� �Լ�
void ATutorialLevelGameMode::BeginPlay()
{
    // �θ� Ŭ������ BeginPlay ȣ��
    Super::BeginPlay();

    // ��� ������ �����Ǿ� �ִٸ�,
    if (BackGroundMusic) {
        // ��� ������ ����Ѵ�. ������ 1.0, ��ġ�� 1.0, ���� �ð��� 0.0
        UGameplayStatics::PlaySound2D(this, BackGroundMusic, 1.f, 1.f, 0.f, nullptr, nullptr, true);
    }
}
