// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/WorldLocation.h"
#include "WorldLocationSubsystem.generated.h"

UCLASS()
class HEREWEGOAGAIN_API UWorldLocationSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void RegisterWorldLocation(const FGameplayTag& LocationId, AWorldLocation* WorldLocation);
	void UnregisterWorldLocation(const FGameplayTag& LocationId, AWorldLocation* WorldLocation);
	AWorldLocation* GetWorldLocation(const FGameplayTag& LocationId) const;
	
private:
	TMultiMap<FGameplayTag, TWeakObjectPtr<AWorldLocation>> RegisteredWorldLocations;
};
