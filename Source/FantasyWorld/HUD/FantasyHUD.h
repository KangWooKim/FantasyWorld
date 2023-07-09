// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FantasyOverlay.h"
#include "FantasyHUD.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API AFantasyHUD : public AHUD
{
	GENERATED_BODY()
	

public :

	// FantasyOverlay를 반환하는 Getter입니다. 인라인 메소드로 선언했습니다.
	// 이 메소드를 통해 FantasyOverlay에 접근할 수 있습니다.
	FORCEINLINE UFantasyOverlay* GetFantasyOverlay() const { return FantasyOverlay;  }

protected:

	// 게임이 시작될 때 호출됩니다.
	virtual void BeginPlay() override;

private:

	// 생성된 FantasyOverlay를 저장하는 변수입니다.
	UPROPERTY()
	UFantasyOverlay* FantasyOverlay;

	UPROPERTY(EditdefaultsOnly, Category = Fantasy)
	TSubclassOf<UFantasyOverlay> FantasyOverlayClass;
};
