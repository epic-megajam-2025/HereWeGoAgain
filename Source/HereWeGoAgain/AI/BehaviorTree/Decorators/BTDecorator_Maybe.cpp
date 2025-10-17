

#include "BTDecorator_Maybe.h"

UBTDecorator_Maybe::UBTDecorator_Maybe()
{
	NodeName = "Maybe";
}

bool UBTDecorator_Maybe::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return FMath::RandRange(0.f, 1.f) <= Probability.GetValue(OwnerComp.GetBlackboardComponent());
}

FString UBTDecorator_Maybe::GetStaticDescription() const
{
	return FString::Printf(TEXT("Execute this node with probability %s%s"),
		*Probability.ToString(), bForceSuccess ? TEXT("\nForce success") : TEXT(""));
}

void UBTDecorator_Maybe::OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult)
{
	Super::OnNodeProcessed(SearchData, NodeResult);
	if (bForceSuccess)
		NodeResult = EBTNodeResult::Succeeded;
}
