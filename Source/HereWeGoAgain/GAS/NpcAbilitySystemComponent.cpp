// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcAbilitySystemComponent.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "AI/Controllers/HwgaAiController.h"
#include "AI/Data/AiDataTypes.h"
#include "AI/Data/AIGameplayTags.h"
#include "AI/Data/HWGAGameplayTags.h"
#include "Characters/HWGABaseCharacter.h"

void UNpcAbilitySystemComponent::NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability,
                                                     const FGameplayTagContainer& FailureReason)
{
	Super::NotifyAbilityFailed(Handle, Ability, FailureReason);
	auto AIController = Cast<AAIController>(CharacterOwner->GetController());
	FAIMessage AIMessage;
	AIMessage.Status = FAIMessage::Failure;
	AIMessage.MessageName = FailureReason.HasTag(HWGAGameplayTags::System_Ability_ActivationFailed_CantAffordCost)
		? AIGameplayTags::AI_BrainMessage_Ability_ActivationFailed_CantAfford.GetTag().GetTagName()
		: AIGameplayTags::AI_BrainMessage_Ability_ActivationFailed_ConditionsNotMet.GetTag().GetTagName();
	
	AIMessage.SetFlag(AI_BRAINMESSAGE_FLAG_IMMEDIATE);
	AIController->GetBrainComponent()->HandleMessage(AIMessage);
}

void UNpcAbilitySystemComponent::OnMoveSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	Super::OnMoveSpeedChanged(OnAttributeChangeData);
	if (auto AIController = Cast<AHwgaAiController>(CharacterOwner->GetController()))
	{
		AIController->OnMoveSpeedChanged(OnAttributeChangeData.NewValue);
	}
	
}
