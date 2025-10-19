// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTService_BehaviorEvaluator_Base.h"
#include "BTService_BehaviorEvaluator_ChaseLaserPointer.generated.h"

class AAttentionTrigger;

// #define LASER_POSITIONS_CONSIDERED_NUM 30

UCLASS()
class HEREWEGOAGAIN_API UBTService_BehaviorEvaluator_ChaseLaserPointer : public UBTService_BehaviorEvaluator_Base
{
	GENERATED_BODY()

private:
	// struct FBTMemory_BehaviorEvaluator_ChaseLaserPointer : public FBTMemory_BehaviorEvaluator_Base
	// {
	// 	TWeakObjectPtr<AAttentionTrigger> CurrentlyTrackedLaserPointer;
	// 	FVector PreviousLaserPositions[LASER_POSITIONS_CONSIDERED_NUM];
	// 	int Head = 0;
	// 	bool bStartFromTail = false;
	//
	// 	const FVector& operator[](int Index) const
	// 	{
	// 		const int ActualIndex = Index < 0 ? Index + LASER_POSITIONS_CONSIDERED_NUM : Index % LASER_POSITIONS_CONSIDERED_NUM;
	// 		return PreviousLaserPositions[ActualIndex];
	// 	}
	//
	// 	FVector& operator[](int Index)
	// 	{
	// 		const int ActualIndex = Index < 0 ? Index + LASER_POSITIONS_CONSIDERED_NUM : Index % LASER_POSITIONS_CONSIDERED_NUM;
	// 		return PreviousLaserPositions[ActualIndex];
	// 	}
	// };

	struct FBTMemory_BehaviorEvaluator_ChaseLaserPointer : public FBTMemory_BehaviorEvaluator_Base
	{
		TWeakObjectPtr<AAttentionTrigger> CurrentlyTrackedLaserPointer;
		FVector LastLaserPosition = FAISystem::InvalidLocation;
		FVector SecondLastLaserPosition = FAISystem::InvalidLocation;
		float AccumulatedDistance = 0.f;

		void AddLaserPosition(const FVector& NewLaserPosition)
		{
			SecondLastLaserPosition = LastLaserPosition;
			LastLaserPosition = NewLaserPosition;
		}

		void Clear()
		{
			CurrentlyTrackedLaserPointer.Reset();
			LastLaserPosition = FAISystem::InvalidLocation;
			SecondLastLaserPosition = FAISystem::InvalidLocation;
			AccumulatedDistance = 0.f;
		}

		bool IsValidForDistance() const { return LastLaserPosition != FAISystem::InvalidLocation; }
		bool IsValidForDot(const FVector& NewLocation) const
		{
			float constexpr DotActivationDistanceThresholdSq = 25.f*25.f;
			return LastLaserPosition != FAISystem::InvalidLocation && SecondLastLaserPosition != FAISystem::InvalidLocation
				&& (LastLaserPosition - SecondLastLaserPosition).SizeSquared() > DotActivationDistanceThresholdSq
				&& (NewLocation - LastLaserPosition).SizeSquared() > DotActivationDistanceThresholdSq;
		};
	};
	
public:
	UBTService_BehaviorEvaluator_ChaseLaserPointer();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual uint16 GetInstanceMemorySize() const override { return sizeof(FBTMemory_BehaviorEvaluator_ChaseLaserPointer); };
	virtual FString GetStaticDescription() const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector OutTargetBBKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttentionTriggerTag;
	
	// Only increment chase utility when accumulated distance change exceeds this value
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaserDistanceChangeThreshold = 100.f;

	// Only increment chase utility when accumulated dot change exceeds this value
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaserDotChangeThreshold = 0.35f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaserDistanceStepUtilityIncrease = 0.2f;

	// if dot product between previous direction and current direction is less than this - increase utility
	// encourages player to do jiggly-wiggly motions to attract the cat
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaserDotStepUtilityIncrease = 0.4f;

	// if a laser is further than this value from the cat - cat doesn't care about it
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LaserPointerRelevanceDistance = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ObservationBase = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FRuntimeFloatCurve LaserObservationDurationToUtilityScaleDependencyCurve;
	
	virtual void FinalizeBehaviorState(UBehaviorTreeComponent* BTComponent) const override;
	virtual void InitiateBehaviorState(UBehaviorTreeComponent* BTComponent) const override;
	
private:
	float UpdatePerception(UBehaviorTreeComponent& OwnerComp, FBTMemory_BehaviorEvaluator_Base* BTMemory) const;
	
};
