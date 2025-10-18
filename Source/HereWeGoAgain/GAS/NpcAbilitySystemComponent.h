// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HWGAAbilitySystemComponent.h"
#include "NpcAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UNpcAbilitySystemComponent : public UHWGAAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	virtual void NotifyAbilityFailed(const FGameplayAbilitySpecHandle Handle, UGameplayAbility* Ability, const FGameplayTagContainer& FailureReason) override;
	virtual void OnMoveSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData) override;
};
