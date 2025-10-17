// Fill out your copyright notice in the Description page of Project Settings.


#include "HWGACharacterMovementComponent.h"

void UHWGACharacterMovementComponent::SetWalkSpeed(float NewValue)
{
	MaxWalkSpeed = NewValue;
}

void UHWGACharacterMovementComponent::SetTurnRate(float NewValue)
{
	RotationRate.Yaw = NewValue;
}
