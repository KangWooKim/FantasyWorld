// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Beacon.generated.h"

UCLASS()
class FANTASYWORLD_API ABeacon : public AActor
{
	GENERATED_BODY()
	
public:
	// 이 액터의 프로퍼티의 기본값을 설정하는 생성자입니다.
	ABeacon();

protected:
	// 게임이 시작되거나 액터가 스폰될 때 호출되는 함수입니다.
	virtual void BeginPlay() override;

public:
	// 매 프레임마다 호출되는 함수입니다.
	// DeltaTime: 이전 프레임과 현재 프레임 사이의 시간 간격
	virtual void Tick(float DeltaTime) override;

	// 액터간에 겹침이 발생했을 때 호출되는 함수입니다.
	// OverlappedActor: 겹침이 발생한 이 액터
	// OtherActor: 겹침이 발생한 다른 액터
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

private:
	// 이 액터에 속한 UCapsuleComponent입니다. 
	// 일반적으로 물리적인 상호작용을 위해 사용됩니다.
	UPROPERTY()
		UCapsuleComponent* CapsuleComponent;

};
