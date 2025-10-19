// Fill out your copyright notice in the Description page of Project Settings.


#include "HwgaAiPerceptionComponent.h"

#include "AIController.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"

UHwgaAiPerceptionComponent::UHwgaAiPerceptionComponent()
{
    PrimaryComponentTick.bStartWithTickEnabled = true;
    PrimaryComponentTick.bCanEverTick = true;
    // PrimaryComponentTick.TickInterval = 0.2f;
}

void UHwgaAiPerceptionComponent::BeginPlay()
{
    Super::BeginPlay();
    // OnTargetPerceptionUpdated.AddDynamic(this, &)
    OnTargetPerceptionInfoUpdated.AddDynamic(this, &UHwgaAiPerceptionComponent::OnTargetPerceptionInfoUpdatedHandler);
    OnTargetPerceptionForgotten.AddDynamic(this, &UHwgaAiPerceptionComponent::OnTargetPerceptionForgottenHandler);
    OnTargetPerceptionUpdated.AddDynamic(this, &UHwgaAiPerceptionComponent::OnTargetPerceptionUpdatedHandler);
}

bool UHwgaAiPerceptionComponent::ConditionallyStoreSuccessfulStimulus(FAIStimulus& StimulusStore,
    const FAIStimulus& NewStimulus)
{
    // Accumulating damage instead of overriding it
    if (NewStimulus.Type == UAISense::GetSenseID(UAISense_Damage::StaticClass()))
    {
        if (StimulusStore.Strength > 0.f && !StimulusStore.IsExpired())
        {
            float InitialStrength = StimulusStore.Strength;
            StimulusStore = NewStimulus;
            StimulusStore.Strength += InitialStrength;
            return true;
        }
    }
    
    return Super::ConditionallyStoreSuccessfulStimulus(StimulusStore, NewStimulus);
}

void UHwgaAiPerceptionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (ActorsObservationTime.Num() > 0)
    {
        TArray<AActor*> CurrentlySeenActors;
        GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), CurrentlySeenActors);
        for (auto& ActorMemory : ActorsObservationTime)
            if (ActorMemory.Key.IsValid() && CurrentlySeenActors.Contains(ActorMemory.Key.Get()))
                ActorMemory.Value += DeltaTime;
    }
}

float UHwgaAiPerceptionComponent::GetAccumulatedTimeSeen(AActor* Actor) const
{
    const auto* ObservationTime = ActorsObservationTime.Find(Actor);
    return ObservationTime ? *ObservationTime : 0.f;
}

float UHwgaAiPerceptionComponent::GetAccumulatedDamage() const
{
    auto DamageSenseId = UAISense::GetSenseID(UAISense_Damage::StaticClass());
    float AccumulatedDamage = 0.f;
    for (auto DataIt = GetPerceptualDataConstIterator(); DataIt; ++DataIt)
    {
        for (const auto& AIStimulus : DataIt.Value().LastSensedStimuli)
        {
            if (AIStimulus.IsExpired() || AIStimulus.Type != DamageSenseId)
                continue;

            AccumulatedDamage += AIStimulus.Strength;
        }
    }

    return AccumulatedDamage;
}

void UHwgaAiPerceptionComponent::OnTargetPerceptionInfoUpdatedHandler(const FActorPerceptionUpdateInfo& UpdateInfo)
{
    if (UpdateInfo.Stimulus.Type == UAISense::GetSenseID(UAISense_Sight::StaticClass()))
    {
        if (UpdateInfo.Stimulus.Strength > 0.f && UpdateInfo.Stimulus.WasSuccessfullySensed())
        {
            if (!ActorsObservationTime.Contains(UpdateInfo.Target))
                ActorsObservationTime.Add(UpdateInfo.Target, 0.f);
            
            // auto& ActorMemory = ActorsMemory.FindOrAdd(UpdateInfo.Target);
            // ActorMemory.TimeSeen = 0.f;
        }
        else if (UpdateInfo.Stimulus.IsExpired())
        {
            ActorsObservationTime.Remove(UpdateInfo.Target);
        }
    }
}

void UHwgaAiPerceptionComponent::OnTargetPerceptionForgottenHandler(AActor* Actor)
{
    ActorsObservationTime.Remove(Actor);
}

void UHwgaAiPerceptionComponent::OnTargetPerceptionUpdatedHandler(AActor* Actor, FAIStimulus Stimulus)
{
    TargetPerceptionUpdatedNativeEvent.Broadcast(Actor, Stimulus);
}