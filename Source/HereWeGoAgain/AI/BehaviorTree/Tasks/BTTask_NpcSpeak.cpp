#include "BTTask_NpcSpeak.h"

#include "AIController.h"
#include "AI/Interfaces/NpcInterface.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Hearing.h"

class UBlackboardKeyType_GameplayTag;

UBTTask_NpcSpeak::UBTTask_NpcSpeak()
{
	NodeName = "Say to NPCs";
}

EBTNodeResult::Type UBTTask_NpcSpeak::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto Controller = OwnerComp.GetAIOwner();
	auto Pawn = Controller->GetPawn();
	
	ensure(AISoundTag.IsValid());
	UAISense_Hearing::ReportNoiseEvent(Pawn, Pawn->GetActorLocation(), Loudness, Pawn, Range, AISoundTag.GetTagName());
	if (auto NpcPawn = Cast<INpcInterface>(Pawn))
		NpcPawn->Say(AISoundTag);
	
	return EBTNodeResult::Succeeded;
}

FString UBTTask_NpcSpeak::GetStaticDescription() const
{
	return FString::Printf(TEXT("Report AI noise event %s\nLoudness = %.2f\nRange = %.2f"),
		*AISoundTag.ToString(), Loudness, Range);
}
