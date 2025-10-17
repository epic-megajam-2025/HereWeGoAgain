// Fill out your copyright notice in the Description page of Project Settings.


#include "HwgaAnimInstance.h"

#include "Characters/HWGABaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UHwgaAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	ControlledCharacter = Cast<AHWGABaseCharacter>(TryGetPawnOwner());
	if (ControlledCharacter.IsValid())
	{
		CMC = ControlledCharacter->GetCharacterMovement();
	}
}

void UHwgaAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!ControlledCharacter.IsValid())
		return;

	Velocity = ControlledCharacter->GetVelocity();
	Speed = Velocity.Size();
	AccelerationDirection = CMC->GetCurrentAcceleration().GetSafeNormal();
	Acceleration = AccelerationDirection.Size();
	bFalling = CMC->IsFalling() || CMC->IsFlying();
	Rotation = ControlledCharacter->GetControlRotation();
	if (Rotation.Pitch > 180.f)
		Rotation.Pitch -= 360.f;

	FVector AccelerationDirection2D = AccelerationDirection.GetSafeNormal2D();
	MoveDirectionYaw = FMath::RadiansToDegrees(FMath::Acos(AccelerationDirection2D | ControlledCharacter->GetActorForwardVector()));
	if ((AccelerationDirection2D | ControlledCharacter->GetActorRightVector()) < 0.f)
		MoveDirectionYaw *= -1.f;
}

void UHwgaAnimInstance::SetFocusPoint(const FVector& NewFocusPoint)
{
	FocusPoint = NewFocusPoint;
}

void UHwgaAnimInstance::ResetFocusPoint()
{
	FocusPoint = FVector::ZeroVector;
}
