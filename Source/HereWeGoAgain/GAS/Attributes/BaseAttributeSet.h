// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectAggregator.h"
#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

struct HEREWEGOAGAIN_API FHWGAAggregatorEvaluateMetaDataLibrary
{
	static FAggregatorEvaluateMetaData UseAllModifiersButOnlyLatestOverride;
};

UCLASS()
class HEREWEGOAGAIN_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override {};
};
