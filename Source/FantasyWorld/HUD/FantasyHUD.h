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

	// FantasyOverlay�� ��ȯ�ϴ� Getter�Դϴ�. �ζ��� �޼ҵ�� �����߽��ϴ�.
	// �� �޼ҵ带 ���� FantasyOverlay�� ������ �� �ֽ��ϴ�.
	FORCEINLINE UFantasyOverlay* GetFantasyOverlay() const { return FantasyOverlay;  }

protected:

	// ������ ���۵� �� ȣ��˴ϴ�.
	virtual void BeginPlay() override;

private:

	// ������ FantasyOverlay�� �����ϴ� �����Դϴ�.
	UPROPERTY()
	UFantasyOverlay* FantasyOverlay;

	UPROPERTY(EditdefaultsOnly, Category = Fantasy)
	TSubclassOf<UFantasyOverlay> FantasyOverlayClass;
};
