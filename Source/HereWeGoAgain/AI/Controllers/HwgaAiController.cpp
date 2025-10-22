// Fill out your copyright notice in the Description page of Project Settings.


#include "HwgaAiController.h"

#include "AbilitySystemInterface.h"
#include "AI/Components/HwgaAiPerceptionComponent.h"
#include "AI/Components/HwgaBehaviorTreeComponent.h"
#include "AI/Components/NpcBehaviorEvaluatorComponent.h"
#include "AI/Components/NpcComponent.h"
#include "AI/Components/NpcStatesComponent.h"
#include "AI/Data/AIGameplayTags.h"
#include "AI/Data/HWGAGAMEPLAYTAGS.h"
#include "AI/Data/NpcBlackboardKeysDataAsset.h"
#include "Animation/HwgaAnimInstance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/HWGABaseCharacter.h"
#include "Game/LogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Subsystems/WorldLocationSubsystem.h"

AHwgaAiController::AHwgaAiController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UHwgaBehaviorTreeComponent>(TEXT("")))
{
	PrimaryActorTick.bCanEverTick = true;
	NpcComponent = CreateDefaultSubobject<UNpcComponent>(TEXT("NpcComponent"));
	NpcStatesComponent = CreateDefaultSubobject<UNpcStatesComponent>(TEXT("NpcStatesComponent"));
	Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard"));
	BrainComponent = CreateDefaultSubobject<UHwgaBehaviorTreeComponent>(TEXT("BrainComponent"));
	PerceptionComponent = CreateDefaultSubobject<UHwgaAiPerceptionComponent>(TEXT("Perception"));
	NpcBehaviorEvaluatorComponent = CreateDefaultSubobject<UNpcBehaviorEvaluatorComponent>(TEXT("NpcBehaviorEvaluatorComponent"));
}

void AHwgaAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (auto ASCInterface = Cast<IAbilitySystemInterface>(InPawn))
		NpcStatesComponent->SetAbilitySystemComponent(ASCInterface->GetAbilitySystemComponent());

	PossessedCharacter = Cast<AHWGABaseCharacter>(InPawn);
	if (!PossessedCharacter.IsValid())
	{
		UE_LOG(LogHWGA, Warning, TEXT("Possessed character is not HWGABaseCharacter"));
		return;
	}
	
	if (RootBehaviorTree == nullptr)
	{
		UE_LOG(LogHWGA, Warning, TEXT("Root behavior tree is not set"));
		return;
	}
	
	RunBehaviorTree(RootBehaviorTree);
	if (IsValid(BlackboardKeys))
	{
		NpcBehaviorEvaluatorComponent->Initialize(Cast<UBehaviorTreeComponent>(GetBrainComponent()), BlackboardKeys);
		
		GetWorld()->GetTimerManager().SetTimerForNextTick([this]
		{
			auto WLS = GetWorld()->GetSubsystem<UWorldLocationSubsystem>();
			auto GameStartLocation = WLS->GetWorldLocation(HWGAGameplayTags::Location_Game_Start);
			auto GameEndLocation = WLS->GetWorldLocation(HWGAGameplayTags::Location_Game_End);
			auto BlackboardComponent = GetBlackboardComponent();
			if (IsValid(GameStartLocation) && !BlackboardKeys->StartLocationBBKey.SelectedKeyName.IsNone())
				BlackboardComponent->SetValueAsVector(BlackboardKeys->StartLocationBBKey.SelectedKeyName, GameStartLocation->GetActorLocation());

			if (IsValid(GameEndLocation) && !BlackboardKeys->EndLocationBBKey.SelectedKeyName.IsNone())
				BlackboardComponent->SetValueAsVector(BlackboardKeys->EndLocationBBKey.SelectedKeyName, GameEndLocation->GetActorLocation());

			NpcBehaviorEvaluatorComponent->RequestEvaluatorsActive(InitiallyActiveBehaviorEvaluators, true);
		});
	}
}

void AHwgaAiController::SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority)
{
	Super::SetFocus(NewFocus, InPriority);
	if (InPriority >= EAIFocusPriority::Gameplay && IsValid(NewFocus))
	{
		UE_VLOG(this, LogAI_Focus, Verbose, TEXT("Got actor focus: %s"), *NewFocus->GetName());
		if (PossessedCharacter.IsValid())
		{
			PossessedCharacter->AddGameplayTag(AIGameplayTags::AI_State_Focused, true);
			if (auto HwgaAnimInstance = Cast<UHwgaAnimInstance>(PossessedCharacter->GetMesh()->GetAnimInstance()))
				HwgaAnimInstance->SetFocusActor(NewFocus);
		}
	}
}

void AHwgaAiController::SetFocalPoint(FVector NewFocus, EAIFocusPriority::Type InPriority)
{
	Super::SetFocalPoint(NewFocus, InPriority);
	if (InPriority >= EAIFocusPriority::Gameplay)
	{
		UE_VLOG(this, LogAI_Focus, Verbose, TEXT("Got new location focus: %s"), *NewFocus.ToString());
		if (PossessedCharacter.IsValid())
		{
			PossessedCharacter->AddGameplayTag(AIGameplayTags::AI_State_Focused, true);
			if (auto HwgaAnimInstance = Cast<UHwgaAnimInstance>(PossessedCharacter->GetMesh()->GetAnimInstance()))
				HwgaAnimInstance->SetFocusPoint(NewFocus);
		}
	}
}

void AHwgaAiController::ClearFocus(EAIFocusPriority::Type InPriority)
{
	Super::ClearFocus(InPriority);
	if (InPriority >= EAIFocusPriority::Gameplay)
	{
		UE_VLOG(this, LogAI_Focus, Verbose, TEXT("Clear focus"));
		if (PossessedCharacter.IsValid())
		{
			PossessedCharacter->RemoveGameplayTag(AIGameplayTags::AI_State_Focused);
			if (auto HwgaAnimInstance = Cast<UHwgaAnimInstance>(PossessedCharacter->GetMesh()->GetAnimInstance()))
				HwgaAnimInstance->ClearFocusPoint();
		}
	}
}

void AHwgaAiController::SetSightRadius(const float NewRadius)
{
	for (auto Itr = PerceptionComponent->GetSensesConfigIterator(); Itr; ++Itr)
	{
		UAISenseConfig* Config = *Itr;
		if(UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(Config))
		{
			SightConfig->SightRadius = NewRadius;
			SightConfig->LoseSightRadius = NewRadius * 1.2f;
			PerceptionComponent->ConfigureSense(*SightConfig);
			break;
		}
	}
}

void AHwgaAiController::SetSightHalfAngle(const float NewHalfAngle)
{
	for (auto Itr = PerceptionComponent->GetSensesConfigIterator(); Itr; ++Itr)
	{
		UAISenseConfig* Config = *Itr;
		if(UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(Config))
		{
			SightConfig->PeripheralVisionAngleDegrees = NewHalfAngle;
			PerceptionComponent->ConfigureSense(*SightConfig);
			break;
		}
	}
}

void AHwgaAiController::SetHearingRadius(const float NewHearingRadius)
{
	for (auto Itr = PerceptionComponent->GetSensesConfigIterator(); Itr; ++Itr)
	{
		UAISenseConfig* Config = *Itr;
		if(UAISenseConfig_Hearing* HearingConfig = Cast<UAISenseConfig_Hearing>(Config))
		{
			HearingConfig->HearingRange =NewHearingRadius;
			PerceptionComponent->ConfigureSense(*HearingConfig);
			break;
		}
	}
}

void AHwgaAiController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if WITH_EDITOR

	// find focus with highest priority
	for (int32 Index = FocusInformation.Priorities.Num() - 1; Index >= 0; --Index)
	{
		const FFocusKnowledge::FFocusItem& FocusItem = FocusInformation.Priorities[Index];
		AActor* FocusActor = FocusItem.Actor.Get();
		if (FocusActor)
		{
			UE_VLOG_LOCATION(this, LogAI_Focus, VeryVerbose, GetFocalPointOnActor(FocusActor), 12, FColor::Yellow,  TEXT("Focus %d. Actor %s "), Index, *FocusActor->GetName());
		}
		else if (FAISystem::IsValidLocation(FocusItem.Position))
		{
			UE_VLOG_LOCATION(this, LogAI_Focus, VeryVerbose, FocusItem.Position, 12, FColor::Yellow,  TEXT("Focus %d"), Index);
		}
	}
#endif
}
