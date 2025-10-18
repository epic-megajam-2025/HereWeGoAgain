// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AITypes.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "NpcComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UNpcComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	FORCEINLINE void SetTargetLocation(const FVector& NewTargetLocation) { CurrentTargetLocation = NewTargetLocation;  }
	FORCEINLINE const FVector& GetTargetLocation() const { return CurrentTargetLocation; };
	FORCEINLINE void ClearTargetLocation() { CurrentTargetLocation = FAISystem::InvalidLocation; };
	
	FORCEINLINE void SetDistanceToTarget(float NewValue) { DistanceToTarget = NewValue; }
	FORCEINLINE float GetDistanceToTarget() const { return DistanceToTarget; }
	
	void StoreTaggedLocation(const FGameplayTag& DataTag, const FVector& Vector);
	void StoreTaggedActor(const FGameplayTag& DataTag, AActor* Actor);

	AActor* GetStoredActor(const FGameplayTag& DataTag, bool bConsumeAfterReading = false);
	FVector GetStoredLocation(const FGameplayTag& DataTag, bool bConsumeAfterReading = false);

	const FRichCurve* GetCatchUpSpeedDependencyCurve() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FRuntimeFloatCurve CatchUpSpeedDependencyCurve;
	
private:
	float DistanceToTarget = 0.f;
	FVector CurrentTargetLocation = FAISystem::InvalidLocation;
	TMap<FGameplayTag, FVector> StoredLocations;
	TMap<FGameplayTag, TWeakObjectPtr<AActor>> StoredActors;
};
