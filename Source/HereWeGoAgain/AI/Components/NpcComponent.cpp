// Fill out your copyright notice in the Description page of Project Settings.

#include "NpcComponent.h"

void UNpcComponent::StoreTaggedLocation(const FGameplayTag& DataTag, const FVector& Vector)
{
	StoredLocations.FindOrAdd(DataTag) = Vector;
}

FVector UNpcComponent::GetStoredLocation(const FGameplayTag& DataTag, bool bConsumeAfterReading)
{
	auto Result = StoredLocations.Contains(DataTag) ? StoredLocations[DataTag] : FAISystem::InvalidLocation;
	if (bConsumeAfterReading)
		StoredLocations.Remove(DataTag);
	
	return Result;
}

const FRichCurve* UNpcComponent::GetCatchUpSpeedDependencyCurve() const
{
	return CatchUpSpeedDependencyCurve.GetRichCurveConst();
}

void UNpcComponent::StoreTaggedActor(const FGameplayTag& DataTag, AActor* Actor)
{
	StoredActors.FindOrAdd(DataTag) = Actor;
}

AActor* UNpcComponent::GetStoredActor(const FGameplayTag& DataTag, bool bConsumeAfterReading)
{
	auto Result = StoredActors.Contains(DataTag) ? StoredActors[DataTag].Get() : nullptr;
	if (bConsumeAfterReading)
		StoredActors.Remove(DataTag);

	return Result;
}