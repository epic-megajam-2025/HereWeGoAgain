// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HWGACharacterMovementComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UHWGACharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	void SetWalkSpeed(float NewValue);
	void SetTurnRate(float NewValue);
};
