// Fill out your copyright notice in the Description page of Project Settings.


#include "HwgaAiController.h"

#include "AbilitySystemInterface.h"
#include "AI/Components/HwgaAiPerceptionComponent.h"
#include "AI/Components/HwgaBehaviorTreeComponent.h"
#include "AI/Components/NpcComponent.h"
#include "AI/Components/NpcStatesComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

AHwgaAiController::AHwgaAiController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UHwgaBehaviorTreeComponent>(TEXT("")))
{
	PrimaryActorTick.bCanEverTick = true;
	NpcComponent = CreateDefaultSubobject<UNpcComponent>(TEXT("NpcComponent"));
	NpcStatesComponent = CreateDefaultSubobject<UNpcStatesComponent>(TEXT("NpcStatesComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UHwgaBehaviorTreeComponent>(TEXT("BrainComponent"));
	PerceptionComponent = CreateDefaultSubobject<UHwgaAiPerceptionComponent>(TEXT("Perception"));
}

void AHwgaAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (auto ASCInterface = Cast<IAbilitySystemInterface>(InPawn))
		NpcStatesComponent->SetAbilitySystemComponent(ASCInterface->GetAbilitySystemComponent());
}
