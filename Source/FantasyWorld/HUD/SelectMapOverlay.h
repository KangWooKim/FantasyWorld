// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectMapOverlay.generated.h"

class UButton;
class ALobbyLevelHUD;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API USelectMapOverlay : public UUserWidget
{
	GENERATED_BODY()
	

public:

    // NativeConstruct �޼��带 �������̵��մϴ�. �̴� ������ ������ �� ȣ��˴ϴ�.
    virtual void NativeConstruct() override;

    // GrassLand �� ��ư�� Ŭ������ �� ȣ��Ǵ� �Լ��Դϴ�.
    UFUNCTION()
        void OnGrassLandButtonClicked();

    // IceLand �� ��ư�� Ŭ������ �� ȣ��Ǵ� �Լ��Դϴ�.
    UFUNCTION()
        void OnIceLandButtonClicked();

    // InfinityMode �� ��ư�� Ŭ������ �� ȣ��Ǵ� �Լ��Դϴ�.
    UFUNCTION()
        void OnInfinityModeButtonClicked();

    // ���� ��ư�� Ŭ������ �� ȣ��Ǵ� �Լ��Դϴ�.
    UFUNCTION()
        void OnQuitButtonClicked();

protected:

private:

    // GrassLand �� ���� ��ư�Դϴ�.
    UPROPERTY(meta = (BindWidget))
        UButton* GrassLandButton;

    // IceLand �� ���� ��ư�Դϴ�.
    UPROPERTY(meta = (BindWidget))
        UButton* IceLandButton;

    // InfinityMode �� ���� ��ư�Դϴ�.
    UPROPERTY(meta = (BindWidget))
        UButton* InfinityModeButton;

    // ���� ��ư�Դϴ�.
    UPROPERTY(meta = (BindWidget))
        UButton* QuitButton;

    // ��ư Ŭ�� �� ����� �����Դϴ�.
    UPROPERTY(EditAnywhere)
        USoundBase* ButtonClickedSound;

    // ���� ���õ� ���� �̸��� �����մϴ�.
    UPROPERTY()
        FString MapName;

    // HUD�� �����մϴ�.
    UPROPERTY()
        ALobbyLevelHUD* HUD;


};
