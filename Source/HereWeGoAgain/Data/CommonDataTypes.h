#pragma once
#include "GameplayEffect.h"

#include "CommonDataTypes.generated.h"

USTRUCT(BlueprintType)
struct FGameplayEffectWrapper
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GameplayEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Level = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> SetByCallerParams;
};

USTRUCT(BlueprintType)
struct FGameplayEffectsContainer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGameplayEffectWrapper> GameplayEffects;
};
