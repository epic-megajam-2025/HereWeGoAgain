// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_SayPhrase.h"

#include "AIController.h"
#include "AI/Interfaces/NpcInterface.h"

UBTService_SayPhrase::UBTService_SayPhrase()
{
	NodeName = "Say phrase";
}

void UBTService_SayPhrase::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	if (PhrasesOptions.IsEmpty())
		return;
	
	if (auto Npc = Cast<INpcInterface>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		const auto& TagsArray = PhrasesOptions.GetGameplayTagArray();
		Npc->Say(TagsArray[FMath::RandRange(0, TagsArray.Num() - 1)]);
	}
}

FString UBTService_SayPhrase::GetStaticDescription() const
{
	return FString::Printf(TEXT("Say one of:\n%s\n%s"), *PhrasesOptions.ToStringSimple(), *Super::GetStaticDescription());
}
