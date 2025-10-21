// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "AISettings.generated.h"

class UCurveVector;
/**
 * 
 */
UCLASS(Config=Game, defaultconfig, DisplayName="AI settings")
class HEREWEGOAGAIN_API UAISettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve NavLinkJumpSpeedDependencyCurve_XY;

	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve NavLinkJumpSpeedDependencyCurve_Z;
};
