// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NpcComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UNpcComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void SetDistanceToTarget(float NewValue) { DistanceToTarget = NewValue; };
	float GetDistanceToTarget() const { return DistanceToTarget; };

private:
	float DistanceToTarget = 0.f;
};
