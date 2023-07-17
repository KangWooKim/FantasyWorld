// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterTypes.h"
#include "BaseCharacter.h"
#include "Countess.generated.h"

class UBoxComponent;
class USoundBase;
class UAnimMontage;
/**
 * 
 */
UCLASS()
class FANTASYWORLD_API ACountess : public ABaseCharacter
{
	GENERATED_BODY()
	
public : 

	void ExecuteGetHit(FHitResult& BoxHit);
	TArray<AActor*> IgnoreActors;
	ACountess();

protected : 


private : 

	void BoxTrace(FHitResult& BoxHit);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	FVector BoxTraceExtent = FVector(5.f);

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	bool bShowBoxDebug = false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBoxFirst;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* WeaponBoxSecond;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStartFirst;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStartSecond;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEndFirst;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEndSecond;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	float Damage = 20.f;
	
};
