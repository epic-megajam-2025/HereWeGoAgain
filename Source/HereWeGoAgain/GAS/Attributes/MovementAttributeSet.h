// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BaseAttributeSet.h"
#include "MovementAttributeSet.generated.h"

UCLASS()
class HEREWEGOAGAIN_API UMovementAttributeSet : public UBaseAttributeSet
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Category = "General")
	FGameplayAttributeData MoveSpeed = 500.f;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, MoveSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "General")
	FGameplayAttributeData TurnRate = 180.f;
	ATTRIBUTE_ACCESSORS(UMovementAttributeSet, TurnRate)

protected:
	virtual void OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute, FAggregator* NewAggregator) const override;
};
