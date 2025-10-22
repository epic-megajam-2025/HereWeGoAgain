// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_BehaviorEvaluator_Flee.h"

#include "AIController.h"
#include "AI/Components/HwgaAiPerceptionComponent.h"
#include "AI/Components/NpcComponent.h"
#include "AI/Data/AIGameplayTags.h"
#include "Characters/HWGABaseCharacter.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"

UBTService_BehaviorEvaluator_Flee::UBTService_BehaviorEvaluator_Flee()
{
	NodeName = "Utility evaluator: flee";
}

void UBTService_BehaviorEvaluator_Flee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto BTMemory = reinterpret_cast<FBTMemory_BehaviorEvaluator_Base*>(NodeMemory);
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	float BehaviorDesireRaw = UpdatePerception(OwnerComp, BTMemory);
	ChangeUtility(BehaviorDesireRaw, Blackboard, DeltaSeconds, BTMemory);
}

float UBTService_BehaviorEvaluator_Flee::UpdatePerception(UBehaviorTreeComponent& OwnerComp,
	FBTMemory_BehaviorEvaluator_Base* BTMemory) const
{
	float Result = 0.f;
	const FAISenseID HearingSenseID = UAISense::GetSenseID(UAISense_Hearing::StaticClass());
	const FAISenseID SightSenseID = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	auto OwnerNpc = OwnerComp.GetAIOwner()->GetPawn();
	FVector NpcLocation = OwnerNpc->GetActorLocation();
	float FleeScale = BTMemory->NpcComponent->GetAttentionTriggerAttractionScale(AIGameplayTags::Attention_Trigger_Flee);
	auto DistanceDependency = DistanceToFleeScaleDependencyCurve.GetRichCurveConst();
	for (auto DataIt = BTMemory->PerceptionComponent->GetPerceptualDataIterator(); DataIt; ++DataIt)
	{
		// scan hearing and damage, collect visual targets
		for (auto& AIStimulus : DataIt.Value().LastSensedStimuli)
		{
			if (!AIStimulus.IsActive() || AIStimulus.IsExpired())
				continue;
			
			if (AIStimulus.Type == HearingSenseID && DataIt->Value.Target != OwnerNpc)
			{
				FGameplayTag SoundTag = FGameplayTag::RequestGameplayTag(AIStimulus.Tag, false);
				if (!ensure(SoundTag.IsValid()))
					continue;
				
				if (SoundTriggers.Contains(SoundTag))
				{
					float DistanceScale = 1.f;
					if (DistanceDependency->HasAnyData())
						DistanceScale = DistanceDependency->Eval((NpcLocation - AIStimulus.StimulusLocation).Size());
					
					Result += SoundTriggers[SoundTag] * DistanceScale;
					AIStimulus.MarkExpired();
				}
			}
			else if (AIStimulus.Type == SightSenseID)
			{
				auto BaseCharacter = Cast<AHWGABaseCharacter>(DataIt.Value().Target);
				if (!BaseCharacter || Enemies.Contains(BaseCharacter->GetCharacterId()))
					continue;

				float DistanceScale = 1.f;
				if (DistanceDependency->HasAnyData())
					DistanceScale = DistanceDependency->Eval((NpcLocation - AIStimulus.StimulusLocation).Size());
				
				Result += Enemies[BaseCharacter->GetCharacterId()] * DistanceScale;
			}
		}
	}
	
	return BTMemory->GetUtilityRegressionOffset() + Result * FleeScale;
}

FString UBTService_BehaviorEvaluator_Flee::GetStaticDescription() const
{
	FString SoundTriggerDescription;
	for (const auto& SoundTrigger : SoundTriggers)
		SoundTriggerDescription += FString::Printf(TEXT("\n%s: %.2f"), *SoundTrigger.Key.ToString(), SoundTrigger.Value);
	
	return FString::Printf(TEXT("React to sounds:%s\n%s"), *SoundTriggerDescription, *Super::GetStaticDescription());
}
