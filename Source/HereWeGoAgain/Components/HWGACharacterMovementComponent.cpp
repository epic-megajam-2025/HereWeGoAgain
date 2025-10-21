// Fill out your copyright notice in the Description page of Project Settings.


#include "HWGACharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Game/LogChannels.h"
#include "GameFramework/Character.h"

void UHWGACharacterMovementComponent::SetWalkSpeed(float NewValue)
{
	MaxWalkSpeed = NewValue;
	if (NewValue > IncreaseDragAtMoveSpeedThreshold && !bIncreasedDrag)
	{
		// TODO (@red) adjust dragging properties
		GroundFriction = 1.f;
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

bool UHWGACharacterMovementComponent::InterpolateToLocation(const FVector& Location, float InterpolationRate, const TArray<const AActor*>& IgnoredActors, bool bSweep)
{
	if (!ensure(Location != FVector::ZeroVector && Location != FAISystem::InvalidLocation))
	{
		return false;
	}
	
	float CapsuleHalfHeight = CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector TraceEndLocation = Location - FVector::UpVector * (CapsuleHalfHeight + 50.f);
	FHitResult FloorHitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActors(IgnoredActors);
	bool bTracedFloor = GetWorld()->LineTraceSingleByChannel(FloorHitResult, Location + FVector::UpVector * 5.f,
		TraceEndLocation,  ECC_Visibility, CollisionQueryParams);
	if (!bTracedFloor)
	{
		UE_VLOG(CharacterOwner, LogHWGA, Warning, TEXT("Failed to project interpolation location to floor"));
		return false;
	}
	
	PreviousMovementMode = MovementMode;
	PreviousCustomMovementMode = CustomMovementMode;
	ForceLocation = FloorHitResult.Location + FVector::UpVector * CapsuleHalfHeight;

	UE_VLOG_CAPSULE(CharacterOwner->GetController(), LogHWGA, VeryVerbose, ForceLocation - CapsuleHalfHeight * FVector::UpVector, CapsuleHalfHeight,
		CharacterOwner->GetCapsuleComponent()->GetScaledCapsuleRadius(), FQuat::Identity, FColor::White, TEXT("Interpolating to"));
	
	ForceLocationRate = InterpolationRate;
	bSweepInterpolation = bSweep;
	SetMovementMode(MOVE_Custom, (uint8)EHWGAMovementMode::Interpolating);
	return true;
}

void UHWGACharacterMovementComponent::UnsetDesiredLocation()
{
	if (ForceLocation != FAISystem::InvalidLocation && CustomMovementMode == (uint8)EHWGAMovementMode::Interpolating && MovementMode == MOVE_Custom)
	{
		ForceLocation = FAISystem::InvalidLocation;
		SetMovementMode(PreviousMovementMode, PreviousCustomMovementMode);
	}
}

void UHWGACharacterMovementComponent::PhysCustom(float deltaTime, int32 Iterations)
{
	Super::PhysCustom(deltaTime, Iterations);
	switch (CustomMovementMode)
	{
	case (uint8)EHWGAMovementMode::Interpolating:
		PhysCustomInterpolate(deltaTime);
		break;
	default:
		break;
	}
}

void UHWGACharacterMovementComponent::PhysCustomInterpolate(float DeltaTime)
{
	FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	FVector NextLocation = FMath::VInterpTo(CurrentLocation, ForceLocation, DeltaTime, ForceLocationRate);
	FHitResult HitResult;
	// MoveAlongFloor((NextLocation - CurrentLocation) / DeltaTime, DeltaTime);
	bool bMoved = SafeMoveUpdatedComponent(NextLocation - CurrentLocation, UpdatedComponent->GetComponentRotation(), bSweepInterpolation, HitResult);
	if (NextLocation.Equals(ForceLocation, 2.f) || (!bMoved && bSweepInterpolation))
	{
		ForceLocation = FAISystem::InvalidLocation;
		SetMovementMode(PreviousMovementMode, PreviousCustomMovementMode);
	}
}