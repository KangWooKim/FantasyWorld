// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyLevelHUD.generated.h"

class ULobbyLevelMenuOverlay;
class USelectMapOverlay;
class USelectCharacterOverlay;
class USettingsOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ALobbyLevelHUD : public AHUD
{
	GENERATED_BODY()
	
public:

    virtual void BeginPlay() override; // ������ ���۵� �� ȣ��Ǵ� �������̵� �Լ��Դϴ�.

    UFUNCTION()
        void CreateSelectMapOverlay(); // �� ���� �������̸� �����ϴ� �Լ��Դϴ�.

    UFUNCTION()
        void ShowSelectMapOverlay(); // �� ���� �������̸� �����ִ� �Լ��Դϴ�.

    UFUNCTION()
        void HideSelectMapOverlay(); // �� ���� �������̸� ����� �Լ��Դϴ�.

    UFUNCTION()
        void CreateSelectCharacterOverlay(); // ĳ���� ���� �������̸� �����ϴ� �Լ��Դϴ�.

    UFUNCTION()
        void ShowSelectCharacterOverlay(); // ĳ���� ���� �������̸� �����ִ� �Լ��Դϴ�.

    UFUNCTION()
        void HideSelectCharacterOverlay(); // ĳ���� ���� �������̸� ����� �Լ��Դϴ�.

    UFUNCTION()
        void CreateSettingsOverlay(); // ���� �������̸� �����ϴ� �Լ��Դϴ�.

    UFUNCTION()
        void ShowSettingsOverlay(); // ���� �������̸� �����ִ� �Լ��Դϴ�.

    UFUNCTION()
        void HideSettingsOverlay(); // ���� �������̸� ����� �Լ��Դϴ�.

protected:
    // ��ӹ��� Ŭ�������� ���� ������ ������� ���⿡ ��ġ��ŵ�ϴ�.

private:

    UWorld* World; // ���忡 ���� �����Դϴ�.
    APlayerController* PlayerController; // �÷��̾� ��Ʈ�ѷ��� ���� �����Դϴ�.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<ULobbyLevelMenuOverlay> LobbyLevelMenuOverlayClass; // �κ� ���� �޴� �������� Ŭ�����Դϴ�.

    UPROPERTY()
        ULobbyLevelMenuOverlay* LobbyLevelMenuOverlay; // �κ� ���� �޴� ���������� �ν��Ͻ��Դϴ�.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<USelectMapOverlay> SelectMapOverlayClass; // �� ���� �������� Ŭ�����Դϴ�.

    UPROPERTY()
        USelectMapOverlay* SelectMapOverlay; // �� ���� ���������� �ν��Ͻ��Դϴ�.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<USelectCharacterOverlay> SelectCharacterOverlayClass; // ĳ���� ���� �������� Ŭ�����Դϴ�.

    UPROPERTY()
        USelectCharacterOverlay* SelectCharacterOverlay; // ĳ���� ���� ���������� �ν��Ͻ��Դϴ�.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<USettingsOverlay> SettingsOverlayClass; // ���� �������� Ŭ�����Դϴ�.

    UPROPERTY()
        USettingsOverlay* SettingsOverlay; // ���� ���������� �ν��Ͻ��Դϴ�.

};
