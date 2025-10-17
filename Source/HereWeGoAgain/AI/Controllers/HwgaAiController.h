// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HwgaAiController.generated.h"

class UNpcComponent;
class UNpcStatesComponent;

UCLASS()
class HEREWEGOAGAIN_API AHwgaAiController : public AAIController
{
	GENERATED_BODY()

public:
	AHwgaAiController(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcStatesComponent* NpcStatesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcComponent* NpcComponent;
};
