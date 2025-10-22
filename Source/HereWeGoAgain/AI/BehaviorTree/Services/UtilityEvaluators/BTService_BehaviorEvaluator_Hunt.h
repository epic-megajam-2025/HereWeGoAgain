// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_BehaviorEvaluator_Base.h"
#include "BTService_BehaviorEvaluator_Hunt.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UBTService_BehaviorEvaluator_Hunt : public UBTService_BehaviorEvaluator_Base
{
	GENERATED_BODY()

public:
	UBTService_BehaviorEvaluator_Hunt();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> Targets;

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve DistanceDependencyCurve;

private:
	float UpdatePerception(UBehaviorTreeComponent& OwnerComp, FBTMemory_BehaviorEvaluator_Base* BTMemory) const;	
};
