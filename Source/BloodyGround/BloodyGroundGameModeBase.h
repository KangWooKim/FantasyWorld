// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BloodyGroundGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class BLOODYGROUND_API ABloodyGroundGameModeBase : public AGameMode
{
	GENERATED_BODY()
	
public:

	ABloodyGroundGameModeBase();

	// �÷��̾ �������ϴ� �Լ� ����
	void RespawnPlayer(APlayerController* PC);
};
