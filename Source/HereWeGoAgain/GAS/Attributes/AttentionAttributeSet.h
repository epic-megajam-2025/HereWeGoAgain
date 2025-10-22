// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "AttentionAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class HEREWEGOAGAIN_API UAttentionAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData LaserPointerAttractionRate = 1.f;
	ATTRIBUTE_ACCESSORS(UAttentionAttributeSet, LaserPointerAttractionRate)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData HuntAttractionRate = 1.f;
	ATTRIBUTE_ACCESSORS(UAttentionAttributeSet, HuntAttractionRate)

	UPROPERTY(BlueprintReadOnly)
	FGameplayAttributeData FearAttractionRate = 1.f;
	ATTRIBUTE_ACCESSORS(UAttentionAttributeSet, FearAttractionRate)
};
