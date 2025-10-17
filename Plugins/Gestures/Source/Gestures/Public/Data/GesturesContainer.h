#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GesturesContainer.generated.h"

USTRUCT(BlueprintType)
struct GESTURES_API FGestureItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachmentSocket = NAME_None;

	// If item is "two handed", specify socket on the static mesh to put left hand on
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName LeftHandPositioningSocketName = NAME_None;

	// If item is "two handed", specify socket on the static mesh to put right hand on
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName RightHandPositioningSocketName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> StaticMesh = nullptr;
};

USTRUCT(BlueprintType)
struct GESTURES_API FGesture
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UAnimMontage* GestureMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName MainLoopMontageSection = FName("LoopStart");
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EndLoopMontageSection = FName("LoopEnd");
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGestureItem> GestureItems;

	// Key - section name of an intermediate montage, value - weight 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.001f, UIMin = 0.001f))
	TMap<FName, float> IntermediateSectionsWeights;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin = 0.001f, UIMin = 0.001f, EditCondition="IntermediateSections.Num() > 0"))
	float MainLoopWeight = 5.f;
    
	// suitable for montages that make the character to stick out of capsule too much. like sitting on a bench
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDisableCollision = false;

	// Is montage supposed to be looped (kinda hacky but perhaps cheaper than looking for a loop in montage sections)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bLooped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bDisableMovementDuringGesture = false;
};

USTRUCT(BlueprintType)
struct FContextGestures
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagQuery CharacterState;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGesture> Gestures;
};

USTRUCT(BlueprintType)
struct GESTURES_API FGestures
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FContextGestures> ContextGestures;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FGesture> Gestures;
};

UCLASS()
class GESTURES_API UGesturesDataAsset : public UDataAsset
{
	GENERATED_BODY()
    
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGestures> GesturesOptions;
};
