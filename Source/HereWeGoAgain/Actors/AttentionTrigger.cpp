// Fill out your copyright notice in the Description page of Project Settings.


#include "AttentionTrigger.h"

#include "Perception/AIPerceptionStimuliSourceComponent.h"

// Sets default values
AAttentionTrigger::AAttentionTrigger()
{
	PerceptionStimuliSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Perception Stimuli Source"));
}

void AAttentionTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void AAttentionTrigger::SetAttentionTriggerActive_Implementation(bool bActive)
{
	bAttentionTriggerActive = bActive;
}
