// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Decorators/BTDecorator_SetState.h"
#include "Components/ActorComponent.h"
#include "Data/CommonDataTypes.h"
#include "NpcStatesComponent.generated.h"


class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UNpcStatesComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void SetStateActive(const FGameplayTag& StateTag, bool bActive);
	void SetAbilitySystemComponent(UAbilitySystemComponent* NewAbilitySystemComponent);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FGameplayEffectsContainer> StateGameplayEffects; 
	
private:
	void ApplyGameplayEffectsForState(const FGameplayTag& StateTag, bool bInActive, const FGameplayEffectsContainer* EffectsContainer);
	
	TMap<FGameplayTag, TArray<FActiveGameplayEffectHandle>> ActiveStateEffects;
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
