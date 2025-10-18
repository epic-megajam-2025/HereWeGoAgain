#include "WorldLocationSubsystem.h"

void UWorldLocationSubsystem::RegisterWorldLocation(const FGameplayTag& LocationId, AWorldLocation* WorldLocation)
{
	RegisteredWorldLocations.Add(LocationId, WorldLocation);
}

void UWorldLocationSubsystem::UnregisterWorldLocation(const FGameplayTag& LocationId, AWorldLocation* WorldLocation)
{
	RegisteredWorldLocations.Remove(LocationId, WorldLocation);
}

AWorldLocation* UWorldLocationSubsystem::GetWorldLocation(const FGameplayTag& LocationId) const
{
	TArray<TWeakObjectPtr<AWorldLocation>> WorldLocations;
	RegisteredWorldLocations.MultiFind(LocationId, WorldLocations);
	return WorldLocations.Num() > 0 ? WorldLocations[0].Get() : nullptr;
}
