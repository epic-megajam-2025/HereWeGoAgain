// Fill out your copyright notice in the Description page of Project Settings.


#include "HwgaBehaviorTreeComponent.h"

#include "AIController.h"
#include "AI/Data/AiDataTypes.h"
#include "Game/LogChannels.h"

void UHwgaBehaviorTreeComponent::HandleMessage(const FAIMessage& Message)
{
	Super::HandleMessage(Message);
	if (Message.HasFlag(AI_BRAINMESSAGE_FLAG_IMMEDIATE))
	{
		UE_VLOG(GetAIOwner(), LogAI, Verbose, TEXT("Handling immediate AI message %s"), *Message.MessageName.ToString());
		HandleMessageImmediately(Message);
	}
	else
	{
		UE_VLOG(GetAIOwner(), LogAI, Verbose, TEXT("Handling non-immediate AI message %s"), *Message.MessageName.ToString());
		Super::HandleMessage(Message);
	}
}

void UHwgaBehaviorTreeComponent::HandleMessageImmediately(const FAIMessage& Message)
{
	for (int32 ObserverIndex = 0; ObserverIndex < MessageObservers.Num(); ObserverIndex++)
	{
		if (MessageObservers[ObserverIndex]->GetObservedMessageType() == Message.MessageName)
		{
			MessageObservers[ObserverIndex]->OnMessage(Message);
			break;
		}
	}
}