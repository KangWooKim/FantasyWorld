// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TutorialLevelGameMode.generated.h"

class USoundBase;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATutorialLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

    ATutorialLevelGameMode();  // �⺻ �������Դϴ�. ���� ����� �ʱ� ������ ���⼭ �� �� �ֽ��ϴ�.
    virtual void BeginPlay() override;  // BeginPlay �Լ��� �������̵��մϴ�. ������ ���۵� �� ȣ��Ǵ� �Լ��Դϴ�.

protected:
    // ��ȣ�� ��� ������ �Լ��� ���⿡ ��ġ�մϴ�.

private:

    UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
        USoundBase* BackGroundMusic;  // ��� ������ ������ �� �ִ� �Ӽ��Դϴ�. �����Ϳ��� �� ���� ������ �� �ֽ��ϴ�.

};
