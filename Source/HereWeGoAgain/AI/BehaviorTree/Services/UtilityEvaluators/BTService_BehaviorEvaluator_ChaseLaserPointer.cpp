// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BehaviorEvaluator_ChaseLaserPointer.h"

#include "AIController.h"
#include "Actors/AttentionTrigger.h"
#include "AI/Components/HwgaAiPerceptionComponent.h"
#include "Game/LogChannels.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_BehaviorEvaluator_ChaseLaserPointer::UBTService_BehaviorEvaluator_ChaseLaserPointer()
{
	NodeName = "Utility evaluator: laser pointer";
	OutTargetBBKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_BehaviorEvaluator_ChaseLaserPointer, OutTargetBBKey), AActor::StaticClass());
}

void UBTService_BehaviorEvaluator_ChaseLaserPointer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto BTMemory = reinterpret_cast<FBTMemory_BehaviorEvaluator_Base*>(NodeMemory);
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	float BehaviorDesireRaw = UpdatePerception(OwnerComp, BTMemory);
	ChangeUtility(BehaviorDesireRaw, Blackboard, DeltaSeconds, BTMemory);
}

float UBTService_BehaviorEvaluator_ChaseLaserPointer::UpdatePerception(UBehaviorTreeComponent& OwnerComp,
                                                                       FBTMemory_BehaviorEvaluator_Base* BTMemory) const
{
	FVector CurrentPawnLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	const float RegressionOffset = BTMemory->GetUtilityRegressionOffset();
	const float RelevantDistanceSq = LaserPointerRelevanceDistance * LaserPointerRelevanceDistance;
	auto ChaseBTMemory = reinterpret_cast<FBTMemory_BehaviorEvaluator_ChaseLaserPointer*>(BTMemory);
	AAttentionTrigger* CurrentLaserPointer = ChaseBTMemory->CurrentlyTrackedLaserPointer.IsValid()
			&& ChaseBTMemory->CurrentlyTrackedLaserPointer->IsAttentionTriggerActive()
			&& (ChaseBTMemory->CurrentlyTrackedLaserPointer->GetActorLocation() - CurrentPawnLocation).SizeSquared() <= RelevantDistanceSq
		? ChaseBTMemory->CurrentlyTrackedLaserPointer.Get()
		: nullptr;
	
	if (CurrentLaserPointer == nullptr)
	{
		UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("No laser pointer known. Looking in perception"));
		TArray<AActor*> PerceivedActors;
		BTMemory->PerceptionComponent->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
		for (const auto& SeenActor : PerceivedActors)
		{
			if (auto AttentionTrigger = Cast<AAttentionTrigger>(SeenActor))
			{
				UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, VeryVerbose, TEXT("In perception: potential attention trigger %s %s"),
					*AttentionTrigger->GetName(), *AttentionTrigger->GetAttentionTriggerTag().ToString());
				const bool bWantToFollow = AttentionTrigger->IsAttentionTriggerActive()
					&& AttentionTrigger->GetAttentionTriggerTag() == AttentionTriggerTag
					&& (AttentionTrigger->GetActorLocation() - CurrentPawnLocation).SizeSquared() <= RelevantDistanceSq;
				
				if (bWantToFollow)
				{
					UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, VeryVerbose, TEXT("Taking laser pointer from perception"));
					CurrentLaserPointer = AttentionTrigger;					
					break;
				}
			}
		}
	}

	auto Blackboard = OwnerComp.GetBlackboardComponent();
	if (CurrentLaserPointer == nullptr)
	{
		if (ChaseBTMemory->CurrentlyTrackedLaserPointer.IsValid())
		{
			UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("Lost laser pointer"));
			UE_VLOG_LOCATION(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose,
				ChaseBTMemory->CurrentlyTrackedLaserPointer->GetActorLocation(), 12.f, FColor::Black, TEXT("Lost laser pointer"));
		}
		else
		{
			UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("No laser pointer"));
		}
		
		ChaseBTMemory->Clear();
		if (ChaseBTMemory->bActive)
			Blackboard->ClearValue(OutTargetBBKey.SelectedKeyName);
		
		return RegressionOffset;
	}

	ChaseBTMemory->CurrentlyTrackedLaserPointer = CurrentLaserPointer;
	auto LaserPointerLocation = CurrentLaserPointer->GetActorLocation();
	UE_VLOG_LOCATION(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, LaserPointerLocation, 12.f, FColor::Red, TEXT("Laser pointer"));

	float ObservationDurationUtilityScale = 1.f;
	float ObservationDuration = BTMemory->PerceptionComponent->GetAccumulatedTimeSeen(CurrentLaserPointer);
	if (auto ObservationDurationUtilityScaleDependency = LaserObservationDurationToUtilityScaleDependencyCurve.GetRichCurveConst())
		ObservationDurationUtilityScale = ObservationDurationUtilityScaleDependency->Eval(ObservationDuration);

	UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, VeryVerbose, TEXT("Observation duration utility scale: %.2f"), ObservationDurationUtilityScale);
	float MotionUtility = 0.f;
	if (ChaseBTMemory->IsValidForDistance())
	{
		ChaseBTMemory->AccumulatedDistance += (LaserPointerLocation - ChaseBTMemory->LastLaserPosition).Size();
		if (ChaseBTMemory->AccumulatedDistance > LaserDistanceStepUtilityIncrease)
		{
			UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("Motion Utility increased by distance step"));
			MotionUtility += LaserDistanceStepUtilityIncrease;
			ChaseBTMemory->AccumulatedDistance = 0.f;
		}
	}

	if (ChaseBTMemory->IsValidForDot(LaserPointerLocation))
	{
		FVector PreviousDirection = (ChaseBTMemory->LastLaserPosition - ChaseBTMemory->SecondLastLaserPosition).GetSafeNormal();
		FVector CurrentDirection = (LaserPointerLocation - ChaseBTMemory->LastLaserPosition).GetSafeNormal();
		const float Dot = PreviousDirection | CurrentDirection;
		if (Dot <= LaserDotStepUtilityIncrease)
		{
			UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("Motion Utility increased by dot step"));
			MotionUtility += LaserDotStepUtilityIncrease;
		}
	}

	ChaseBTMemory->AddLaserPosition(LaserPointerLocation);
	
	if (BTMemory->bActive)
	{
		if (Blackboard->GetValueAsObject(OutTargetBBKey.SelectedKeyName) != CurrentLaserPointer)
			Blackboard->SetValueAsObject(OutTargetBBKey.SelectedKeyName, CurrentLaserPointer);

		UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("Updated target in BB"));
	}

	const float Result = RegressionOffset + MotionUtility + ObservationBase * ObservationDurationUtilityScale;
	UE_VLOG(OwnerComp.GetAIOwner(), LogAIUtility_ChaseLaser, Verbose, TEXT("Result = %.2f"), Result);
	return Result;
}

void UBTService_BehaviorEvaluator_ChaseLaserPointer::InitiateBehaviorState(UBehaviorTreeComponent* BTComponent) const
{
	Super::InitiateBehaviorState(BTComponent);
	auto BTMemory = reinterpret_cast<FBTMemory_BehaviorEvaluator_Base*>(BTComponent->GetNodeMemory(this, BTComponent->FindInstanceContainingNode(this)));
	UpdatePerception(*BTComponent, BTMemory);
}

void UBTService_BehaviorEvaluator_ChaseLaserPointer::FinalizeBehaviorState(UBehaviorTreeComponent* BTComponent) const
{
	Super::FinalizeBehaviorState(BTComponent);
	if (BTComponent)
		if (auto Blackboard = BTComponent->GetBlackboardComponent())
			Blackboard->SetValueAsObject(OutTargetBBKey.SelectedKeyName, nullptr);
}

FString UBTService_BehaviorEvaluator_ChaseLaserPointer::GetStaticDescription() const
{
	return FString::Printf(TEXT("[out]Target BB: %s\nAttention trigger tag: %s\n%s"),
		*OutTargetBBKey.SelectedKeyName.ToString(), *AttentionTriggerTag.ToString(),
		*Super::GetStaticDescription());
}
