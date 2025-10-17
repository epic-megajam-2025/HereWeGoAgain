// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Data/GesturesContainer.h"
#include "GameplayAbilityTargetData_Gesture.generated.h"

USTRUCT(BlueprintType)
struct GESTURES_API FGameplayAbilityTargetData_Gesture : public FGameplayAbilityTargetData
{
	GENERATED_BODY()
	
public:
	FGameplayAbilityTargetData_Gesture();
	
	UPROPERTY()
	FGameplayTag GestureTag;

	UPROPERTY()
	TMap<FGameplayTag, FGestureItem> ExternalGestureItems;

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FGameplayAbilityTargetData_Gesture::StaticStruct();
	}

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
	{
		// The engine already defined NetSerialize for FName & FPredictionKey, thanks Epic!
		GestureTag.NetSerialize(Ar, Map, bOutSuccess);
		// 13.01.2025 @AK: TODO: if network support is needed - convert ExternalGestureItems to an array where maps key is a part of array item. Arrays can be serialized
		bOutSuccess = true;
		return true;
	}
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_Gesture> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_Gesture>
{
	enum
	{
		WithNetSerializer = true // This is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};
