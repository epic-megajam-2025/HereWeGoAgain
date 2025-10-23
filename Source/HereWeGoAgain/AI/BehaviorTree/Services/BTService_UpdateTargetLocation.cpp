// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateTargetLocation.h"

#include "AIController.h"
#include "AI/Components/NpcComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateTargetLocation::UBTService_UpdateTargetLocation()
{
	NodeName = "Update target location";
	TargetBBKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, TargetBBKey), AActor::StaticClass());
	CanSeeTargetBBKey.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, CanSeeTargetBBKey));
	OutTargetLocationBBKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateTargetLocation, OutTargetLocationBBKey));
	bNotifyCeaseRelevant = true;
}

void UBTService_UpdateTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto Blackboard = OwnerComp.GetBlackboardComponent();
	auto Target = Cast<AActor>(Blackboard->GetValueAsObject(TargetBBKey.SelectedKeyName));
	auto NpcComponent = OwnerComp.GetAIOwner()->FindComponentByClass<UNpcComponent>();
	bool bCanSeeTarget = Blackboard->GetValueAsBool(CanSeeTargetBBKey.SelectedKeyName);
	if (!Target || !bCanSeeTarget)
	{
		if (bClearLocationOnTargetLost)
		{
			Blackboard->ClearValue(OutTargetLocationBBKey.SelectedKeyName);
			NpcComponent->ClearTargetLocation();
		}
		
		return;
	}

	NpcComponent->SetTargetLocation(Target->GetActorLocation(), DeltaSeconds);
	Blackboard->SetValueAsVector(OutTargetLocationBBKey.SelectedKeyName, Target->GetActorLocation());
}

FString UBTService_UpdateTargetLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Target BB: %s\nCan see target BB: %s\n[out]Target location BB: %s\n%s"),
		*TargetBBKey.SelectedKeyName.ToString(), *CanSeeTargetBBKey.SelectedKeyName.ToString(), *OutTargetLocationBBKey.SelectedKeyName.ToString(),
		*Super::GetStaticDescription());
}

void UBTService_UpdateTargetLocation::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (bClearLocationOnCeaseRelevant)
	{
		if (auto AIController = OwnerComp.GetAIOwner())
			if (auto NpcComponent = AIController->FindComponentByClass<UNpcComponent>())
				NpcComponent->ClearTargetLocation();

		if (auto Blackboard = OwnerComp.GetBlackboardComponent())
			Blackboard->ClearValue(OutTargetLocationBBKey.SelectedKeyName);
	}
	
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}
