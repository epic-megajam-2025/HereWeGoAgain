// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_IsActorCurrentTarget.h"

#include "AIController.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_ActorBase.h"

UEnvQueryTest_IsActorCurrentTarget::UEnvQueryTest_IsActorCurrentTarget()
{
	ValidItemType = UEnvQueryItemType_ActorBase::StaticClass();
	Cost = EEnvTestCost::Type::Low;
	SetWorkOnFloatValues(true);
}

void UEnvQueryTest_IsActorCurrentTarget::RunTest(FEnvQueryInstance& QueryInstance) const
{
	auto Pawn = Cast<APawn>(QueryInstance.Owner.Get());
	if (Pawn == nullptr)
		return;

	auto AIController = Cast<AAIController>(Pawn->GetController());
	if (AIController == nullptr)
		return;

	auto FocusTarget = AIController->GetFocusActor();
	if (FocusTarget == nullptr)
		return;

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		auto TestActor = GetItemActor(QueryInstance, It.GetIndex());
		It.SetScore(TestPurpose, FilterType, TestActor == FocusTarget, true);
	}
}
