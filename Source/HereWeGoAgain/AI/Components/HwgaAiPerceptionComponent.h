// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Perception/AIPerceptionComponent.h"
#include "HwgaAiPerceptionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UHwgaAiPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

	private:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FTargetPerceptionUpdatedNativeDelegate, AActor* TriggerActor, const FAIStimulus& Stimulus);
	
public:
	UHwgaAiPerceptionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	float GetAccumulatedTimeSeen(AActor* Actor) const;

	UFUNCTION(BlueprintCallable)
	float GetAccumulatedDamage() const;
	
	FTargetPerceptionUpdatedNativeDelegate TargetPerceptionUpdatedNativeEvent;

protected:
	virtual void BeginPlay() override;
	virtual bool ConditionallyStoreSuccessfulStimulus(FAIStimulus& StimulusStore, const FAIStimulus& NewStimulus) override;

private:
	UFUNCTION()
	void OnTargetPerceptionInfoUpdatedHandler(const FActorPerceptionUpdateInfo& UpdateInfo);

	UFUNCTION()
	void OnTargetPerceptionForgottenHandler(AActor* Actor);

	UFUNCTION()
	void OnTargetPerceptionUpdatedHandler(AActor* Actor, FAIStimulus Stimulus);

	TMap<TWeakObjectPtr<AActor>, float> ActorsObservationTime;
};
