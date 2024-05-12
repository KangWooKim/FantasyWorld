// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "MachineGun.generated.h"

/**
 * 
 */
UCLASS()
class BLOODYGROUND_API AMachineGun : public ABaseWeapon
{
	GENERATED_BODY()
	
public:

	AMachineGun();

	UFUNCTION()
	virtual void Fire() override;

	virtual EWeaponType GetCurrentWeaponType() override;

	virtual void PerformReload() override;

	virtual void ChangeWeapon() override;

protected:

	UFUNCTION()
	virtual bool CanFire() override;

	virtual void FireEnd() override;

	UFUNCTION(BluePrintCallable)
	void ShootEnd();

private:

	FTimerHandle FireTimerHandle; // ���� �߻縦 ���� Ÿ�̸� �ڵ�
	float FireRate;               // �߻� ����

};
