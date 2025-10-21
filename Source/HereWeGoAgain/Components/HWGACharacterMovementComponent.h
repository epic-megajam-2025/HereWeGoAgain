// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HWGACharacterMovementComponent.generated.h"

UENUM()
enum class EHWGAMovementMode : uint8
{
	None = 0,
	Interpolating = 1
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UHWGACharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void SetWalkSpeed(float NewValue);
	void SetTurnRate(float NewValue);

	bool InterpolateToLocation(const FVector& Vector, float InterpolationRate, const TArray<const AActor*>& IgnoredActors, bool bSweep);
	void UnsetDesiredLocation();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float IncreaseDragAtMoveSpeedThreshold = 450.f;

private:
	bool bIncreasedDrag = false;
	FVector InterpolationDestination = FAISystem::InvalidLocation;

protected:
	virtual void PhysCustom(float deltaTime, int32 Iterations) override;
	
private:
	bool bForceLocation = false;
	FVector ForceLocation = FVector::ZeroVector;
	float ForceLocationRate = 0.f;
	bool bSweepInterpolation = true;

	TEnumAsByte<EMovementMode> PreviousMovementMode;
	
	uint8 PreviousCustomMovementMode;

	bool bHasForcedMoveSpeed = false;

	void PhysCustomInterpolate(float DeltaTime);
};
