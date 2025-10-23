// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SayPhrase.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UBTService_SayPhrase : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_SayPhrase();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer PhrasesOptions;
};
