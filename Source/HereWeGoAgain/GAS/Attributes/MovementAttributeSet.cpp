// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementAttributeSet.h"

void UMovementAttributeSet::OnAttributeAggregatorCreated(const FGameplayAttribute& Attribute,
	FAggregator* NewAggregator) const
{
	Super::OnAttributeAggregatorCreated(Attribute, NewAggregator);
	if (Attribute == GetMoveSpeedAttribute() || Attribute == GetTurnRateAttribute())
	{
		NewAggregator->EvaluationMetaData = &FHWGAAggregatorEvaluateMetaDataLibrary::UseAllModifiersButOnlyLatestOverride;
	}
}
