#pragma once


// ĳ������ �׼� ���¸� ��Ÿ���� ������
UENUM(BlueprintType)
enum class EActionState : uint8
{
	EAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	EAS_HitReaction UMETA(DisplayName = "HitReaction"),
	EAS_Attacking UMETA(DisplayName = "Attacking"),
	EAS_EquippingWeapon UMETA(DisplayName = "Equipping Weapon"),
	EAS_Dodge UMETA(DisplayName = "Dodge"),
	EAS_Dead UMETA(DisplayName = "Dead")
};

// ���� �ִϸ��̼� ��� ��Ÿ���� ������
UENUM(BlueprintType)
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_Death2 UMETA(DisplayName = "Death2"),
	EDP_Death3 UMETA(DisplayName = "Death3"),
	EDP_Death4 UMETA(DisplayName = "Death4"),
	EDP_Death5 UMETA(DisplayName = "Death5"),
	EDP_Death6 UMETA(DisplayName = "Death6"),

	EDP_MAX UMETA(DisplayName = "DefaultMAX")
};

// ���� ���¸� ��Ÿ���� ������
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_NoState UMETA(DisplayName = "NoState"),
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")

};

// �ʻ�� ��忡 ���� �ߴ°� ���θ� �ľ��ϱ� ���� ����ϴ� ������
UENUM(BlueprintType)
enum class ELethalState : uint8
{
	ELS_Off UMETA(DisplayName = "Off"),
	ELS_On UMETA(DisplayName = "On"),
	ELS_Max UMETA(DisplayName = "DefaultMAX")
};