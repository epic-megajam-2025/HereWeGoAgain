// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbility_Gesture.generated.h"

class UAbilityTask_WaitGameplayEvent;
class UBaseGesturesComponent;
class UGesturesDataAsset;
/**
 * 
 */
UCLASS()
class GESTURES_API UGameplayAbility_Gesture : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UGameplayAbility_Gesture();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void CancelAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateCancelAbility) override;
	
protected:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

private:
	UPROPERTY()
	UAbilityTask_WaitGameplayEvent* WaitForAbortEvent;
	
	TWeakObjectPtr<UBaseGesturesComponent> GesturesComponent;
	FTimerHandle GestureEndDelayTimer;

	void OnGestureFinished();

	UFUNCTION()
	void OnGestureAborted(FGameplayEventData Payload);
};
