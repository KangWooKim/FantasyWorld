// Fill out your copyright notice in the Description page of Project Settings.


// "CombatCameraShake.h" ��� ������ ���Խ�ŵ�ϴ�.
#include "CombatCameraShake.h"

// UCombatCameraShake Ŭ������ �⺻ �����ڸ� �����մϴ�.
UCombatCameraShake::UCombatCameraShake()
{
    // ���� ���� �ð��� 1�ʷ� �����մϴ�.
    OscillationDuration = 1.0f;

    // ��ġ(Pitch)�� ���� ũ�⸦ 0���� �����մϴ�. ��, ��ġ �������δ� ī�޶� �������� �ʽ��ϴ�.
    RotOscillation.Pitch.Amplitude = 0.0f;

    // ��(Yaw)�� ���� ũ�⸦ ������ ��(-5.0f ~ 5.0f)���� �����մϴ�. ī�޶� �¿�� �ణ ������ ���Դϴ�.
    RotOscillation.Yaw.Amplitude = FMath::RandRange(-5.0f, 5.0f);

    // ��(Roll)�� ���� ũ�⸦ 0���� �����մϴ�. ��, �� �������δ� ī�޶� �������� �ʽ��ϴ�.
    RotOscillation.Roll.Amplitude = 0.0f;
}
