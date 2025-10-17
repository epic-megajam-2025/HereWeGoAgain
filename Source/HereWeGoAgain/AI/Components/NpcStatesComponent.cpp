// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcStatesComponent.h"

#include "AbilitySystemComponent.h"
#include "Data/CommonDataTypes.h"
#include "Game/LogChannels.h"

void UNpcStatesComponent::SetAbilitySystemComponent(UAbilitySystemComponent* NewAbilitySystemComponent)
{
	if (AbilitySystemComponent.IsValid() && !ActiveStateEffects.IsEmpty())
	{
		UE_VLOG(GetOwner(), LogAI_NpcStates, Warning, TEXT("Error. Attempt to reassign ASC for UNpcStatesComponent when it has active states"));
		ensure(false);
		return;
	}
	
	AbilitySystemComponent = NewAbilitySystemComponent;
}

void UNpcStatesComponent::SetStateActive(const FGameplayTag& StateTag, bool bActive)
{
	auto OwnerLocal = GetOwner();
	if (bActive && ActiveStateEffects.Contains(StateTag))
	{
		UE_VLOG(OwnerLocal, LogAI_NpcStates, Verbose, TEXT("Not activating state %s because it is already active"), *StateTag.ToString());
		return;
	}
	else if (!bActive && !ActiveStateEffects.Contains(StateTag))
	{
		UE_VLOG(OwnerLocal, LogAI_NpcStates, Verbose, TEXT("Not deactivating state %s because it is not active"), *StateTag.ToString());
	}
	
	if (const auto* StateEffectsContainer = StateGameplayEffects.Find(StateTag))
	{
		if (!ensure(!StateEffectsContainer->GameplayEffects.IsEmpty()))
		{
			UE_VLOG(GetOwner(), LogAI_NpcStates, Verbose, TEXT("Not found NPC state %s"), *StateTag.ToString());
			return;
		}
		
		ApplyGameplayEffectsForState(StateTag, bActive, StateEffectsContainer);
	}
}

void UNpcStatesComponent::ApplyGameplayEffectsForState(const FGameplayTag& StateTag,
                                                       bool bInActive, const FGameplayEffectsContainer* EffectsContainer)
{
	auto OwnerLocal = GetOwner();
	if (!ensure(AbilitySystemComponent.IsValid()))
		return;

	if (bInActive)
	{
		TArray<FActiveGameplayEffectHandle>& ActiveEffectHandles = ActiveStateEffects.FindOrAdd(StateTag);
			
		for (const auto& ParametrizedGameplayEffect : EffectsContainer->GameplayEffects )
		{
			if (!IsValid(ParametrizedGameplayEffect.GameplayEffectClass))
			{
				UE_VLOG(OwnerLocal, LogAI_NpcStates, Warning, TEXT("Invalid gameplay effect class for state %s"), *StateTag.ToString());
				continue;
			}
					
			auto EffectContext = AbilitySystemComponent->MakeEffectContext();
			auto EffectCDO = ParametrizedGameplayEffect.GameplayEffectClass->GetDefaultObject<UGameplayEffect>();
			ensure(EffectCDO->DurationPolicy != EGameplayEffectDurationType::Instant); // I believe instant effects aren't really states
			auto EffectSpec = AbilitySystemComponent->MakeOutgoingSpec(ParametrizedGameplayEffect.GameplayEffectClass, ParametrizedGameplayEffect.Level,
				EffectContext);
			EffectSpec.Data->SetByCallerTagMagnitudes.Append(ParametrizedGameplayEffect.SetByCallerParams);
			auto AppliedEffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpec.Data);
			ActiveEffectHandles.Add(AppliedEffectHandle);
		}
				
		UE_VLOG(OwnerLocal, LogAI_NpcStates, Verbose, TEXT("Activated NPC state %s"), *StateTag.ToString());
	}
	else
	{
		if (TArray<FActiveGameplayEffectHandle>* ActiveEffectHandles = ActiveStateEffects.Find(StateTag))
		{
			for (FActiveGameplayEffectHandle& ActiveGE : *ActiveEffectHandles)
			{
				AbilitySystemComponent->RemoveActiveGameplayEffect(ActiveGE);
			}

			UE_VLOG(OwnerLocal, LogAI_NpcStates, Verbose, TEXT("Removed NPC state %s"), *StateTag.ToString());
		}
		ActiveStateEffects.Remove(StateTag);
	}
}

