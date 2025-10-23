// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "WorldLocationBase.h"
#include "GameFramework/Actor.h"
#include "WorldLocation.generated.h"

UCLASS(Blueprintable)
class HEREWEGOAGAIN_API AWorldLocation : public AWorldLocationBase
{
	GENERATED_BODY()

public:
	AWorldLocation();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag LocationTag;
};
