// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateMoveSpeed.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_UpdateMoveSpeed::UBTService_UpdateMoveSpeed()
{
	NodeName = "Update move speed";
	OutMoveSpeedBBKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UBTService_UpdateMoveSpeed, OutMoveSpeedBBKey));
}

void UBTService_UpdateMoveSpeed::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!Pawn)
		return;

	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(OutMoveSpeedBBKey.SelectedKeyName, Pawn->GetVelocity().Size());
}

FString UBTService_UpdateMoveSpeed::GetStaticDescription() const
{
	return FString::Printf(TEXT("[out]Move speed BB: %s\n%s"), *OutMoveSpeedBBKey.SelectedKeyName.ToString(), *Super::GetStaticDescription());
}
