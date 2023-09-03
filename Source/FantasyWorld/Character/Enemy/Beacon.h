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
	// �� ������ ������Ƽ�� �⺻���� �����ϴ� �������Դϴ�.
	ABeacon();

protected:
	// ������ ���۵ǰų� ���Ͱ� ������ �� ȣ��Ǵ� �Լ��Դϴ�.
	virtual void BeginPlay() override;

public:
	// �� �����Ӹ��� ȣ��Ǵ� �Լ��Դϴ�.
	// DeltaTime: ���� �����Ӱ� ���� ������ ������ �ð� ����
	virtual void Tick(float DeltaTime) override;

	// ���Ͱ��� ��ħ�� �߻����� �� ȣ��Ǵ� �Լ��Դϴ�.
	// OverlappedActor: ��ħ�� �߻��� �� ����
	// OtherActor: ��ħ�� �߻��� �ٸ� ����
	UFUNCTION()
		void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);

private:
	// �� ���Ϳ� ���� UCapsuleComponent�Դϴ�. 
	// �Ϲ������� �������� ��ȣ�ۿ��� ���� ���˴ϴ�.
	UPROPERTY()
		UCapsuleComponent* CapsuleComponent;

};
