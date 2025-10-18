// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HwgaAnimInstance.generated.h"

class UCharacterMovementComponent;
class AHWGABaseCharacter;
/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UHwgaAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector AccelerationDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MoveDirectionYaw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector FocusPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float Acceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bFalling;
	
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void SetFocusPoint(const FVector& NewFocusPoint);
	void SetFocusActor(const AActor* FocusActor);
	void ClearFocusPoint();
	
private:
	TWeakObjectPtr<AHWGABaseCharacter> ControlledCharacter;
	TWeakObjectPtr<UCharacterMovementComponent> CMC;
	TWeakObjectPtr<const AActor> FocusActor;
};
