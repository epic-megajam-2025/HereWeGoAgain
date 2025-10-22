// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BehaviorEvaluator_Hunt.h"

#include "AIController.h"
#include "AI/Components/HwgaAiPerceptionComponent.h"
#include "AI/Components/NpcComponent.h"
#include "AI/Data/AIGameplayTags.h"
#include "Characters/HWGABaseCharacter.h"
#include "Perception/AISense_Sight.h"

UBTService_BehaviorEvaluator_Hunt::UBTService_BehaviorEvaluator_Hunt()
{
	NodeName = "Utility evaluator: hunt";
}

void UBTService_BehaviorEvaluator_Hunt::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto BTMemory = reinterpret_cast<FBTMemory_BehaviorEvaluator_Base*>(NodeMemory);
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	float BehaviorDesireRaw = UpdatePerception(OwnerComp, BTMemory);
	ChangeUtility(BehaviorDesireRaw, Blackboard, DeltaSeconds, BTMemory);	
}

float UBTService_BehaviorEvaluator_Hunt::UpdatePerception(UBehaviorTreeComponent& OwnerComp,
	FBTMemory_BehaviorEvaluator_Base* BTMemory) const
{
	float Result = 0.f;
	const FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	auto OwnerNpc = OwnerComp.GetAIOwner()->GetPawn();
	FVector NpcLocation = OwnerNpc->GetActorLocation();
	const auto DistanceDependency = DistanceDependencyCurve.GetRichCurveConst();
	float HuntScale = BTMemory->NpcComponent->GetAttentionTriggerAttractionScale(AIGameplayTags::Attention_Trigger_Hunt);
	for (auto DataIt = BTMemory->PerceptionComponent->GetPerceptualDataIterator(); DataIt; ++DataIt)
	{
		auto BaseCharacter = Cast<AHWGABaseCharacter>(DataIt.Value().Target.Get());
		if (!BaseCharacter)
			continue;

		auto CharacterId = BaseCharacter->GetCharacterId();
		if (!Targets.Contains(CharacterId))
			continue;
		
		// scan hearing and damage, collect visual targets
		for (auto& AIStimulus : DataIt.Value().LastSensedStimuli)
		{
			if (AIStimulus.Type != SightSenseID)
				continue;

			Result += Targets[CharacterId] * DistanceDependency->Eval((NpcLocation - AIStimulus.StimulusLocation).Size());
			break;
		}
	}
	
	return BTMemory->GetUtilityRegressionOffset() + Result * HuntScale;
}

FString UBTService_BehaviorEvaluator_Hunt::GetStaticDescription() const
{
	FString HuntTargetDescription;
	for (const auto Target : Targets)
		HuntTargetDescription += FString::Printf(TEXT("\n%s: %.2f"), *Target.Key.ToString(), Target.Value);
	
	return FString::Printf(TEXT("Hunt these:%s\n%s"), *HuntTargetDescription, *Super::GetStaticDescription());
}
