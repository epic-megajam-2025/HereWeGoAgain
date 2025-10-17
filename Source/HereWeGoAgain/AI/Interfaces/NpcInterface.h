// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Tasks/BTTask_NpcSpeak.h"
#include "UObject/Interface.h"
#include "NpcInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UNpcInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HEREWEGOAGAIN_API INpcInterface
{
	GENERATED_BODY()

public:
	virtual void Say(const FGameplayTag& Phrase) { }
	virtual bool PerformNpcGesture(const FGameplayTag& GestureTag) { return false; };
	virtual void StopNpcGesture() {};
};
