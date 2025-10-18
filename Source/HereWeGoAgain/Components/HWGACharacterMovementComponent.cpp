// Fill out your copyright notice in the Description page of Project Settings.


#include "HWGACharacterMovementComponent.h"

void UHWGACharacterMovementComponent::SetWalkSpeed(float NewValue)
{
	MaxWalkSpeed = NewValue;
	if (NewValue > IncreaseDragAtMoveSpeedThreshold && !bIncreasedDrag)
	{
		// TODO (@red) adjust dragging properties
		GroundFriction = 2.f;
		bIncreasedDrag = true;
	}
	else if (NewValue <= IncreaseDragAtMoveSpeedThreshold && bIncreasedDrag)
	{
		// TODO (@red) adjust dragging properties
		GroundFriction = 8.f;
		bIncreasedDrag = false;
	}
}

void UHWGACharacterMovementComponent::SetTurnRate(float NewValue)
{
	RotationRate.Yaw = NewValue;
}
