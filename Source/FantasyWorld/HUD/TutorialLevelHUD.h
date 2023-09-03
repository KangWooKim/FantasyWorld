// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TutorialLevelHUD.generated.h"

class UTutorialLevelOverlay;

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ATutorialLevelHUD : public AHUD
{
	GENERATED_BODY()

		// public ������ ��� �Լ��� ���� ����
public:

	// ATutorialLevelHUD Ŭ������ �⺻ ������
	ATutorialLevelHUD();

	// BeginPlay �Լ��� �������̵�. ���� �÷��̰� ���۵� �� �� �� ȣ���
	virtual void BeginPlay() override;

	// TutorialLevelOverlay ��� ������ ��ȯ�ϴ� ������ �ζ��� �Լ�
	// FORCEINLINE�� �����Ϸ����� �����ϸ� �ζ������� ������ ����
	FORCEINLINE UTutorialLevelOverlay* GetTutorialLevelOverlay() { return TutorialLevelOverlay; }

	// protected ����. ��ӹ��� Ŭ�������� ���� ����
protected:

	// private ����. �ش� Ŭ���� ���ο����� ���� ����
private:

	// ���� ������ World ��ü�� �����ϴ� ������
	UWorld* World;

	// ���� ������ PlayerController ��ü�� �����ϴ� ������
	APlayerController* PlayerController;

	// TutorialLevelOverlayClass�� Ÿ�� ������ ����. 
	// EditDefaultsOnly�� �����Ϳ����� ����Ʈ ���� ������ �� �ְ� �Ѵ�.
	UPROPERTY(EditDefaultsOnly, Category = "Media")
		TSubclassOf<UTutorialLevelOverlay> TutorialLevelOverlayClass;

	// TutorialLevelOverlay ��ü�� �����ϴ� ������.
	UPROPERTY()
		UTutorialLevelOverlay* TutorialLevelOverlay;

};
