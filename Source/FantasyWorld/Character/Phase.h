// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Phase.generated.h"

/**
 * 
 */
UCLASS()
class FANTASYWORLD_API APhase : public ABaseCharacter
{
	GENERATED_BODY()

		// APhase Ŭ������ public ���� ����
public:

	// APhase Ŭ������ �⺻ �������Դϴ�.
	APhase();

	// UInputComponent�� �����ϴ� ���� �Լ��� �������̵��մϴ�.
	// PlayerInputComponent: �� Ŭ������ ����� �Է� ������Ʈ�Դϴ�.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// �������� ó���ϴ� ���� �Լ��� �������̵��մϴ�.
	// DamageAmount: �޴� ������ ��
	// DamageEvent: ������ �̺�Ʈ ���� (������ Ÿ�� ��)
	// EventInstigator: �������� �ִ� ��Ʈ�ѷ�
	// DamageCauser: �������� �ִ� ����
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// APhase Ŭ������ protected ���� ����
protected:

	// ���� ���۽� �� ���� ȣ��Ǵ� �Լ��Դϴ�.
	virtual void BeginPlay() override;

	// �����Ӹ��� ȣ��Ǵ� �Լ��Դϴ�.
	// DeltaTime: ���� �����Ӱ��� �ð� ����
	virtual void Tick(float DeltaTime) override;

	// �浹 �Ǵ� ���� �� ȣ��Ǵ� �Լ��� �����Դϴ�.
	// ImpactPoint: ������ �Ǵ� �浹�� �߻��� ������ ��ġ
	// Hitter: ���� �Ǵ� �浹�� ����Ų ����
	virtual void GetHit_Implementation(const FVector& ImpactPoint, AActor* Hitter) override;

	// ������ �����ϴ� �Լ��Դϴ�.
	virtual void Attack() override;

	// �ʻ�� ��带 Ȱ��ȭ �Ǵ� ��Ȱ��ȭ�ϴ� �Լ��Դϴ�.
	virtual void LethalMode() override;

	// �ʻ�� ��带 �����ϴ� �Լ��Դϴ�.
	virtual void LethalModeFinish() override;

	// APhase Ŭ������ private ���� ����
private:

	// �ʻ�� ��带 Ȱ��ȭ�� �� �Һ�Ǵ� ��ȥ�� ���� �����մϴ�.
	// �Ϲ������� ������ �����˴ϴ�. (-50�� 50 ��ȥ�� �Һ��Ѵٴ� �ǹ�)
	UPROPERTY(EditAnywhere)
		int32 SoulSpent = -50;

	// �ʻ�� ��� Ȱ��ȭ �� ������ ���� ����Ʈ�Դϴ�.
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* ShieldEffect;

	// ���� ���� �� ��Ÿ�� ����Ʈ�Դϴ�.
	UPROPERTY(EditAnywhere, Category = "Combat")
		UParticleSystem* AttackEffect;

};
