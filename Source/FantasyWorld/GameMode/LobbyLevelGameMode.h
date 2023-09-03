// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyLevelGameMode.generated.h"

class USoundBase;
class UFileMediaSource;
class UMediaPlayer;
class UAudioComponent;
class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ALobbyLevelGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:

    // �⺻ ������
    ALobbyLevelGameMode();

    // BeginPlay �������̵�: ���� ���� �� ȣ��Ǵ� �Լ�
    virtual void BeginPlay() override;

    // Tick �������̵�: �����Ӹ��� ȣ��Ǵ� �Լ�, DeltaTime�� ���� �����Ӱ��� �ð� ����
    virtual void Tick(float DeltaTime) override;

protected:

    // protected ����� ���⿡ ��ġ��

private:

    // ��� ���ǿ� ����� ����. �����Ϳ��� ���� ����
    UPROPERTY(EditAnywhere, Category = "BackGroundMusic")
        USoundBase* BackGroundMusic;

    // ��� ������ ����ϱ� ���� ����� ������Ʈ
    UPROPERTY()
        UAudioComponent* BGMComponent;

    // ���� ������ �÷��̾� ��Ʈ�ѷ� �ν��Ͻ�
    UPROPERTY()
        APlayerController* PlayerController;

    // ������ �ν��Ͻ�. ���⼭�� UMyGameInstance Ÿ������ ���
    UPROPERTY()
        UMyGameInstance* GameInstance;

};
