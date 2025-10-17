// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "HwgaBehaviorTreeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UHwgaBehaviorTreeComponent : public UBehaviorTreeComponent
{
	GENERATED_BODY()

public:
	virtual void HandleMessage(const FAIMessage& Message) override;

private:
	void HandleMessageImmediately(const FAIMessage& Message);
};
