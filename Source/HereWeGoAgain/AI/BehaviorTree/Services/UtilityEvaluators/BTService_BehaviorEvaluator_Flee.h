// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_BehaviorEvaluator_Base.h"
#include "BTService_BehaviorEvaluator_Flee.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UBTService_BehaviorEvaluator_Flee : public UBTService_BehaviorEvaluator_Base
{
	GENERATED_BODY()

private:
	struct FBTMemory_BehaviorEvaluator_Flee : public FBTMemory_BehaviorEvaluator_Base
	{
		TWeakObjectPtr<AActor> LastTriggeredActor;
	};
	
public:
	UBTService_BehaviorEvaluator_Flee();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTMemory_BehaviorEvaluator_Flee); };
	virtual FString GetStaticDescription() const override;

	virtual void InitiateBehaviorState(UBehaviorTreeComponent* BTComponent) const override;
	virtual void FinalizeBehaviorState(UBehaviorTreeComponent* BTComponent) const override;
	
protected:
	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> SoundTriggers;

	UPROPERTY(EditAnywhere)
	TMap<FGameplayTag, float> Enemies;

	UPROPERTY(EditAnywhere)
	FRuntimeFloatCurve DistanceToFleeScaleDependencyCurve;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector OutAttractionActorBBKey;
	
private:
	float UpdatePerception(UBehaviorTreeComponent& OwnerComp, FBTMemory_BehaviorEvaluator_Base* BTMemory) const;
};
