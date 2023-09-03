// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FantasyPlayerController.generated.h"

class UMyGameInstance;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AFantasyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

    // BeginPlay()�� ������ ���۵Ǹ� �� ���� ȣ��Ǵ� �Լ�
    // �� �Լ��� �������̵��ؼ� �÷��̾ ���ӿ� �������� �� �ʱ� ������ �� �� �ִ�.
    virtual void BeginPlay() override;

    // PlayerTick()�� �����Ӹ��� ȣ��Ǵ� �Լ�
    // DeltaTime ���ڴ� ���� �����Ӱ� ���� ������ ������ �ð��� �� ������ ��Ÿ����.
    virtual void PlayerTick(float DeltaTime) override;

    // RePossess()�� ĳ������ �������� �ٽ� ���� �� ȣ���ϴ� �Լ�
    // ���� ���, ĳ���Ͱ� �׾��ٰ� ��Ȱ���� �� �� �Լ��� ����� �� �ִ�.
    UFUNCTION()
        void RePossess();

    // GetPawn()�� ���� �÷��̾� ��Ʈ�ѷ��� �����ϰ� �ִ� ��(ĳ����)�� ��ȯ�ϴ� �Լ�
    UFUNCTION()
        APawn* GetPawn();

    // SetMouseSensitivity()�� ���콺�� �ΰ����� �����ϴ� �Լ�
    // Sensitivity ���ڴ� ���� ������ ���콺 �ΰ��� ��
    UFUNCTION()
        void SetMouseSensitivity(float Sensitivity);

    // GetMouseSensitivity()�� ���� ������ ���콺 �ΰ����� ��ȯ�ϴ� �Լ�
    // FORCEINLINE�� �����Ϸ��� �� �Լ��� �ζ������� ������ �����ϴ� Ű����
    FORCEINLINE float GetMouseSensitivity() const { return MouseSensitivity; }

protected:
    

private:

    // MyPawn�� ���� �÷��̾� ��Ʈ�ѷ��� �����ϰ� �ִ� ��(ĳ����)�� ����
    UPROPERTY()
        APawn* MyPawn;

    // MouseSensitivity�� ���콺�� �ΰ����� �����ϴ� ����, �ʱⰪ�� 1.f
    UPROPERTY(EditAnywhere)
        float MouseSensitivity = 1.f;

    // GameInstance�� ���� ���� �ν��Ͻ��� ����
    // UMyGameInstance Ÿ���� ��ü�� ������ �� �־� ���� ������ �����̳� ���¸� ������ �� �ִ�.
    UPROPERTY()
        UMyGameInstance* GameInstance;

};
