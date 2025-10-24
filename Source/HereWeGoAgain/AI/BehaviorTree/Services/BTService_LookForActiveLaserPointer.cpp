// copyright Bitrat


#include "BTService_LookForActiveLaserPointer.h"

#include "AIController.h"
#include "Actors/AttentionTrigger.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_LookForActiveLaserPointer::UBTService_LookForActiveLaserPointer()
{
	NodeName = "Look for active laser pointer";
	OutLaserPointerBBKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_LookForActiveLaserPointer, OutLaserPointerBBKey), AActor::StaticClass());
}

void UBTService_LookForActiveLaserPointer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr)
		return;

	TArray<AActor*> PerceivedActors;
	AIController->GetAIPerceptionComponent()->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), PerceivedActors);
	for (const auto& SeenActor : PerceivedActors)
	{
		if (auto AttentionTrigger = Cast<AAttentionTrigger>(SeenActor))
		{
			const bool bWantToFollow = AttentionTrigger->IsAttentionTriggerActive()
				&& AttentionTrigger->GetAttentionTriggerTag() == AttentionTriggerTag;
			if (bWantToFollow)
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(OutLaserPointerBBKey.SelectedKeyName, AttentionTrigger);
				return;
			}
		}
	}
}

FString UBTService_LookForActiveLaserPointer::GetStaticDescription() const
{
	return FString::Printf(TEXT("[out]Laser pointer BB: %s\n%s"), *OutLaserPointerBBKey.SelectedKeyName.ToString(),
		*Super::GetStaticDescription());
}
