// 


#include "BTService_CanSeeActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

UBTService_CanSeeActor::UBTService_CanSeeActor()
{
	NodeName = "Update can see actor";
	ActorBBKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CanSeeActor, ActorBBKey), AActor::StaticClass());
	OutCanSeeActorBBKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_CanSeeActor, OutCanSeeActorBBKey));
}

void UBTService_CanSeeActor::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(OutCanSeeActorBBKey.SelectedKeyName, CanSee(OwnerComp));
}

bool UBTService_CanSeeActor::CanSee(UBehaviorTreeComponent& OwnerComp) const
{
	auto Actor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(ActorBBKey.SelectedKeyName));
	if (!Actor)
		return false;
	
	auto AIPerception = OwnerComp.GetAIOwner()->GetAIPerceptionComponent();
	FActorPerceptionBlueprintInfo ActorPerceptionInfo;
	bool bHasPerception = AIPerception->GetActorsPerception(Actor, ActorPerceptionInfo);
	if (!bHasPerception)
		return false;

	auto SightSenseId = UAISense::GetSenseID(UAISense_Sight::StaticClass());
	for (const auto& Stimuli : ActorPerceptionInfo.LastSensedStimuli)
		if (Stimuli.Type == SightSenseId)
			return Stimuli.IsActive();// && !Stimuli.IsExpired(); // IsExpired check might be redundant

	return false;
}

FString UBTService_CanSeeActor::GetStaticDescription() const
{
	return FString::Printf(TEXT("Actor: %s\n[out]Can See BB: %s\n%s"),
		*ActorBBKey.SelectedKeyName.ToString(), *OutCanSeeActorBBKey.SelectedKeyName.ToString(), *Super::GetStaticDescription()) ;
}
