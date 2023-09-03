// Fill out your copyright notice in the Description page of Project Settings.


// "CombatCameraShake.h" 헤더 파일을 포함시킵니다.
#include "CombatCameraShake.h"

// UCombatCameraShake 클래스의 기본 생성자를 정의합니다.
UCombatCameraShake::UCombatCameraShake()
{
    // 진동 지속 시간을 1초로 설정합니다.
    OscillationDuration = 1.0f;

    // 피치(Pitch)의 진동 크기를 0으로 설정합니다. 즉, 피치 방향으로는 카메라가 움직이지 않습니다.
    RotOscillation.Pitch.Amplitude = 0.0f;

    // 요(Yaw)의 진동 크기를 랜덤한 값(-5.0f ~ 5.0f)으로 설정합니다. 카메라가 좌우로 약간 움직일 것입니다.
    RotOscillation.Yaw.Amplitude = FMath::RandRange(-5.0f, 5.0f);

    // 롤(Roll)의 진동 크기를 0으로 설정합니다. 즉, 롤 방향으로는 카메라가 움직이지 않습니다.
    RotOscillation.Roll.Amplitude = 0.0f;
}
