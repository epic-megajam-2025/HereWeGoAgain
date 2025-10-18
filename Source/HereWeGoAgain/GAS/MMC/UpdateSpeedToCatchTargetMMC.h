// 

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "UObject/Object.h"
#include "UpdateSpeedToCatchTargetMMC.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UUpdateSpeedToCatchTargetMMC : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;
};
