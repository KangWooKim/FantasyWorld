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

    // NativeConstruct 메서드를 오버라이딩합니다. 이는 위젯이 구성될 때 호출됩니다.
    virtual void NativeConstruct() override;

    // GrassLand 맵 버튼이 클릭됐을 때 호출되는 함수입니다.
    UFUNCTION()
        void OnGrassLandButtonClicked();

    // IceLand 맵 버튼이 클릭됐을 때 호출되는 함수입니다.
    UFUNCTION()
        void OnIceLandButtonClicked();

    // InfinityMode 맵 버튼이 클릭됐을 때 호출되는 함수입니다.
    UFUNCTION()
        void OnInfinityModeButtonClicked();

    // 종료 버튼이 클릭됐을 때 호출되는 함수입니다.
    UFUNCTION()
        void OnQuitButtonClicked();

protected:

private:

    // GrassLand 맵 선택 버튼입니다.
    UPROPERTY(meta = (BindWidget))
        UButton* GrassLandButton;

    // IceLand 맵 선택 버튼입니다.
    UPROPERTY(meta = (BindWidget))
        UButton* IceLandButton;

    // InfinityMode 맵 선택 버튼입니다.
    UPROPERTY(meta = (BindWidget))
        UButton* InfinityModeButton;

    // 종료 버튼입니다.
    UPROPERTY(meta = (BindWidget))
        UButton* QuitButton;

    // 버튼 클릭 시 재생할 사운드입니다.
    UPROPERTY(EditAnywhere)
        USoundBase* ButtonClickedSound;

    // 현재 선택된 맵의 이름을 저장합니다.
    UPROPERTY()
        FString MapName;

    // HUD를 저장합니다.
    UPROPERTY()
        ALobbyLevelHUD* HUD;


};
