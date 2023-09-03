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

    virtual void BeginPlay() override; // 게임이 시작될 때 호출되는 오버라이드 함수입니다.

    UFUNCTION()
        void CreateSelectMapOverlay(); // 맵 선택 오버레이를 생성하는 함수입니다.

    UFUNCTION()
        void ShowSelectMapOverlay(); // 맵 선택 오버레이를 보여주는 함수입니다.

    UFUNCTION()
        void HideSelectMapOverlay(); // 맵 선택 오버레이를 숨기는 함수입니다.

    UFUNCTION()
        void CreateSelectCharacterOverlay(); // 캐릭터 선택 오버레이를 생성하는 함수입니다.

    UFUNCTION()
        void ShowSelectCharacterOverlay(); // 캐릭터 선택 오버레이를 보여주는 함수입니다.

    UFUNCTION()
        void HideSelectCharacterOverlay(); // 캐릭터 선택 오버레이를 숨기는 함수입니다.

    UFUNCTION()
        void CreateSettingsOverlay(); // 설정 오버레이를 생성하는 함수입니다.

    UFUNCTION()
        void ShowSettingsOverlay(); // 설정 오버레이를 보여주는 함수입니다.

    UFUNCTION()
        void HideSettingsOverlay(); // 설정 오버레이를 숨기는 함수입니다.

protected:
    // 상속받은 클래스에서 접근 가능한 멤버들을 여기에 위치시킵니다.

private:

    UWorld* World; // 월드에 대한 참조입니다.
    APlayerController* PlayerController; // 플레이어 컨트롤러에 대한 참조입니다.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<ULobbyLevelMenuOverlay> LobbyLevelMenuOverlayClass; // 로비 레벨 메뉴 오버레이 클래스입니다.

    UPROPERTY()
        ULobbyLevelMenuOverlay* LobbyLevelMenuOverlay; // 로비 레벨 메뉴 오버레이의 인스턴스입니다.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<USelectMapOverlay> SelectMapOverlayClass; // 맵 선택 오버레이 클래스입니다.

    UPROPERTY()
        USelectMapOverlay* SelectMapOverlay; // 맵 선택 오버레이의 인스턴스입니다.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<USelectCharacterOverlay> SelectCharacterOverlayClass; // 캐릭터 선택 오버레이 클래스입니다.

    UPROPERTY()
        USelectCharacterOverlay* SelectCharacterOverlay; // 캐릭터 선택 오버레이의 인스턴스입니다.

    UPROPERTY(EditDefaultsOnly, Category = "Menu")
        TSubclassOf<USettingsOverlay> SettingsOverlayClass; // 설정 오버레이 클래스입니다.

    UPROPERTY()
        USettingsOverlay* SettingsOverlay; // 설정 오버레이의 인스턴스입니다.

};
