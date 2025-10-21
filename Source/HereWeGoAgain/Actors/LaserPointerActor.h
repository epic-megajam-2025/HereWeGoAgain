// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttentionTrigger.h"
#include "Perception/AISightTargetInterface.h"
#include "LaserPointerActor.generated.h"

UCLASS()
class HEREWEGOAGAIN_API ALaserPointerActor : public AAttentionTrigger, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	ALaserPointerActor();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(MakeEditWidget))
	TArray<FVector> VisibilityChecksOffsets;
	
public: // IAISightTargetInterface
	virtual UAISense_Sight::EVisibilityResult CanBeSeenFrom(const FCanBeSeenFromContext& Context, FVector& OutSeenLocation,
		int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
		float& OutSightStrength, int32* UserData = nullptr, const FOnPendingVisibilityQueryProcessedDelegate* Delegate = nullptr) override;
};
