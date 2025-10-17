// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/GameplayAbility_Gesture.h"

#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Components/BaseGesturesComponent.h"
#include "Data/GestureDataTypes.h"
#include "Data/GestureTags.h"
#include "GAS/GameplayAbilityTargetData_Gesture.h"
#include "Interfaces/GestureCharacter.h"

UGameplayAbility_Gesture::UGameplayAbility_Gesture()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::Type::InstancedPerActor;
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		// Add the ability trigger tag as default to the CDO.
		AbilityTriggers.Reset();
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = GestureGameplayTags::Gesture_Ability_Gesture_Event_Activate;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UGameplayAbility_Gesture::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	// TODO handle cases when a gesture is not found for the provided tag
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	auto GestureCharacter = Cast<IGestureCharacter>(ActorInfo->AvatarActor.Get());
	if (GestureCharacter == nullptr)
	{
		ensure(false);
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	bool bGameLoading = GestureCharacter->IsGameLoading_Gesture();

	bool bGestureStarted = false;
	if (!bGameLoading)
	{
		if (TriggerEventData == nullptr)
		{
			ensure(false);
			EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
			return;
		}
		
		const auto Data = TriggerEventData->TargetData.Get(0);
		if (ensure(Data))
		{
			if (ensure(Data->GetScriptStruct() == FGameplayAbilityTargetData_Gesture::StaticStruct()))
			{
				const FGameplayAbilityTargetData_Gesture* ActivationData = static_cast<const FGameplayAbilityTargetData_Gesture*>(Data);
				FGestureResult GestureResult = ActivationData->ExternalGestureItems.IsEmpty()
					? GesturesComponent->TryStartGesture(ActivationData->GestureTag)
					: GesturesComponent->TryStartGesture(ActivationData->GestureTag, ActivationData->ExternalGestureItems);

				bGestureStarted = GestureResult.bStarted;
			}
		}
	}

	if (bGestureStarted || bGameLoading)
	{
		WaitForAbortEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GestureGameplayTags::Gesture_Ability_Gesture_Event_Abort);
		WaitForAbortEvent->EventReceived.AddDynamic(this, &UGameplayAbility_Gesture::OnGestureAborted);
		WaitForAbortEvent->ReadyForActivation();
				
		return;
	}
	
	UE_VLOG(ActorInfo->AvatarActor.Get(), LogGesture, Verbose, TEXT("Something went wrong. Can't activate gesture ability"));
	GestureCharacter->GestureFinished(false);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
}

void UGameplayAbility_Gesture::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	if (ActorInfo->AvatarActor.IsValid())
	{
		UE_VLOG(ActorInfo->AvatarActor.Get(), LogGesture, Verbose, TEXT("UGameplayAbility_Gesture::EndAbility"));
	}
	
	if (WaitForAbortEvent)
	{
		WaitForAbortEvent->EventReceived.RemoveAll(this);
		WaitForAbortEvent = nullptr;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGameplayAbility_Gesture::CancelAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateCancelAbility)
{
	if (IsActive())
	{
		GesturesComponent->TryStopGesture(true);
		UE_VLOG(ActorInfo->AvatarActor.Get(), LogGesture, Verbose, TEXT("UGameplayAbility_Gesture::CancelAbility"));
	}
	
	Super::CancelAbility(Handle, ActorInfo, ActivationInfo, bReplicateCancelAbility);
}

void UGameplayAbility_Gesture::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                             const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);
	GesturesComponent = ActorInfo->AvatarActor->FindComponentByClass<UBaseGesturesComponent>();
	GesturesComponent->OnGestureFinishedEvent.AddUObject(this, &UGameplayAbility_Gesture::OnGestureFinished);
	ensure(GesturesComponent.IsValid());
}

void UGameplayAbility_Gesture::OnGestureFinished()
{
	UE_VLOG(CurrentActorInfo->AvatarActor.Get(), LogGesture, Verbose, TEXT("UGameplayAbility_Gesture::OnGestureFinished"));	
	EndAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true, false);
}

// use cases:
// 1. player started moving for full body static montages
// 2. AI suddenly decides to do something else instead of a gesture (dodge attack/attack/equip weapon, etc)
void UGameplayAbility_Gesture::OnGestureAborted(FGameplayEventData Payload)
{
	UE_VLOG(CurrentActorInfo->AvatarActor.Get(), LogGesture, Verbose, TEXT("UGameplayAbility_Gesture::OnGestureAborted"));
	if (ensure(GesturesComponent.IsValid()))
		GesturesComponent->TryStopGesture(false);
}
