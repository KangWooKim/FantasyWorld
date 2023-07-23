// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatCameraShake.h"

UCombatCameraShake::UCombatCameraShake()
{
	OscillationDuration = 1.0f;
	RotOscillation.Pitch.Amplitude = 0.0f;
	RotOscillation.Yaw.Amplitude = FMath::RandRange(-5.0f, 5.0f);
	RotOscillation.Roll.Amplitude = 0.0f;

}
