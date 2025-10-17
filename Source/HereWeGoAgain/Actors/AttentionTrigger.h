// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AttentionTrigger.generated.h"

class UAIPerceptionStimuliSourceComponent;

UCLASS()
class HEREWEGOAGAIN_API AAttentionTrigger : public AActor
{
	GENERATED_BODY()

public:
	AAttentionTrigger();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetAttentionAccumulationRate() const { return AttentionAccumulationRate; }

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAttentionTriggerActive(bool bActive);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsAttentionTriggerActive() const { return bAttentionTriggerActive; }
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionStimuliSourceComponent* PerceptionStimuliSourceComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttentionAccumulationRate = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bAttentionTriggerActive = false;
};
