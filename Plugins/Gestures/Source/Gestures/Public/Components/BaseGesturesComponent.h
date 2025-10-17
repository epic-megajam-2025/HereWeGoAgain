#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Data/GestureDataTypes.h"
#include "Data/GesturesContainer.h"
#include "BaseGesturesComponent.generated.h"

class IGestureCharacter;
// TODO consider converting the component to GameplayAbility
class UGesturesDataAsset;
struct FGestureResult;

DECLARE_LOG_CATEGORY_EXTERN(LogGesture, Verbose, All)


UCLASS(Abstract)
class GESTURES_API UBaseGesturesComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	DECLARE_MULTICAST_DELEGATE(FOnGestureFinishedEvent);
	
public:
	UBaseGesturesComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual FGestureResult TryStartGesture(const FGameplayTag& GestureTag);
	virtual FGestureResult TryStartGesture(const FGameplayTag& GestureTag, const TMap<FGameplayTag, FGestureItem>& ExternalGestureItems);
	
	virtual float TryStopGesture(bool bForce);
	const FGameplayTag& GetActiveGestureTag() const;
	void AttachGestureItem(const FGameplayTag& ItemTag, bool bAttach);
	void OnMontageSectionEnded() const;
	float GetActiveGestureEndDuration() const;

	mutable FOnGestureFinishedEvent OnGestureFinishedEvent;

	void OnGameSaved();
	void OnGameLoaded();
	
protected:
	virtual void BeginPlay() override;
	
	TWeakObjectPtr<ACharacter> CharacterOwner;
	
	UPROPERTY()
	TScriptInterface<IGestureCharacter> GestureCharacter;

	UPROPERTY(SaveGame)
	FGameplayTag ActiveGestureTag;

	UPROPERTY(SaveGame)
	int ActiveContextGestureIndex = -1;

	UPROPERTY(SaveGame)
	int ActiveGestureIndex = -1;

	UPROPERTY(SaveGame)
	float ActiveGestureEndTime = 0.f;

	UPROPERTY(SaveGame)
	TMap<FGameplayTag, FGestureAttachedItemData> GestureAttachedItems;

	UPROPERTY(SaveGame)
	float NonLoopedGestureRemainingTime = 0.f;

	UPROPERTY(SaveGame)
	float EndGestureDelayRemainingTime = 0.f;

	UPROPERTY(SaveGame)
	bool bDisableMovementDuringGesture = false;
	
	//@AK 20.09.2024 TODO cache TSoftObjectPtr instead
	UPROPERTY()
	UGesturesDataAsset* CachedGestures;

	virtual void OnGestureStarted(bool bDisableMovementDuringGesture);
	virtual void OnGestureFinished();
	
private:
	ECollisionEnabled::Type InitialMeshCollisionEnabledType;
	ECollisionEnabled::Type InitialCapsuleCollisionEnabledType;

	const TArray<FGesture>* SelectGestureOptions(const FGameplayTag& GestureTag);
	bool GetActiveGesture(FGesture& OutGesture) const;
	void NonLoopedMontageFinished();
	void CleanAttachedGestureItemsHandsPositionings(const FGestureAttachedItemData* AttachedItem);

	FTimerHandle NonLoopedGestureTimer;
	FTimerHandle EndGestureDelayTimer;

	UPROPERTY(SaveGame)
	TMap<FGameplayTag, FGestureItem> ExternalGestureItems;
};