#pragma once

#include "GameplayTagContainer.h"
#include "AiDataTypes.generated.h"

#define AI_BRAINMESSAGE_FLAG_IMMEDIATE 0x01

UENUM()
enum class ENpcTargetDistanceEvaluation : uint8
{
	TargetIsStationary,
	TargetIsApproaching,
	TargetIsGettingAway,
};

USTRUCT(BlueprintType)
struct FBehaviorEvaluatorBlockRequest
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag BehaviorEvaluatorTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIndefinitely = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(EditCondition="bIndefinitely==false"))
	float Duration = 10.f;
};
