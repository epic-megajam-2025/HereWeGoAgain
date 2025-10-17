// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "GestureCharacter.generated.h"

class UGesturesDataAsset;
// This class does not need to be modified.
UINTERFACE()
class UGestureCharacter : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GESTURES_API IGestureCharacter
{
	GENERATED_BODY()

public:
	virtual void GestureStarted() = 0;
	virtual void GestureFinished(bool bSuccess) = 0;
	virtual UGesturesDataAsset* GetGestures() const = 0;
	virtual void SetMovementEnabledForGestures(bool bEnabled) = 0;
	virtual void GetGestureCharacterActiveTags(FGameplayTagContainer& OutTags) const = 0;

	virtual void SetRightHandGesturePositioning(const FVector& RightHandWorldLocation) = 0;
	virtual void SetLeftHandGesturePositioning(const FVector& LeftHandWorldLocation) = 0;
	virtual void ResetRightHandGesturePositioning() = 0;
	virtual void ResetLeftHandGesturePositioning() = 0;
	virtual bool IsGameLoading_Gesture() const = 0;
};
