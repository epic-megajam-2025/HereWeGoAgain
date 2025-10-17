#include "BTTask_Gesture.h"
#include "AIController.h"
#include "NativeGameplayTags.h"
#include "AI/Data/AIGameplayTags.h"
#include "AI/Interfaces/NpcInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Gesture::UBTTask_Gesture()
{
	NodeName = "Gesture";
}

EBTNodeResult::Type UBTTask_Gesture::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Npc = Cast<INpcInterface>(OwnerComp.GetAIOwner()->GetPawn());
	FGameplayTag ActualGestureTag = GestureActionTag;
	
	if (Npc && Npc->PerformNpcGesture(ActualGestureTag))
	{
		if (bAwaitCompletion)
		{
			Super::ExecuteTask(OwnerComp, NodeMemory);
			return EBTNodeResult::InProgress;
		}

		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

void UBTTask_Gesture::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	CompletedMessageTag = AIGameplayTags::AI_BrainMessage_Gesture_Completed;
}

EBTNodeResult::Type UBTTask_Gesture::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (auto Npc = Cast<INpcInterface>(OwnerComp.GetAIOwner()->GetPawn()))
		Npc->StopNpcGesture();
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

FString UBTTask_Gesture::GetStaticDescription() const
{
	return bAwaitCompletion
		? FString::Printf(TEXT("Perform gesture %s and wait\n%s" ), *GestureActionTag.ToString(), *Super::GetStaticDescription())
		: FString::Printf(TEXT("Perform gesture %s\n%s" ), *GestureActionTag.ToString(), *Super::GetStaticDescription());
}
