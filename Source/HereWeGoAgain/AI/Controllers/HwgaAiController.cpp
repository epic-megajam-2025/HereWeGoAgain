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

	OnMoveSpeedChanged(PossessedCharacter->GetCharacterMovement()->MaxWalkSpeed);
}

void AHwgaAiController::SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority)
{
	Super::SetFocus(NewFocus, InPriority);
	if (InPriority >= EAIFocusPriority::Gameplay)
	{
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
		if (PossessedCharacter.IsValid())
		{
			PossessedCharacter->RemoveGameplayTag(AIGameplayTags::AI_State_Focused);
			if (auto HwgaAnimInstance = Cast<UHwgaAnimInstance>(PossessedCharacter->GetMesh()->GetAnimInstance()))
				HwgaAnimInstance->ClearFocusPoint();
		}
	}
}

void AHwgaAiController::OnMoveSpeedChanged(float NewValue)
{
	if (BlackboardKeys == nullptr || BlackboardKeys->MoveSpeedBBKey.SelectedKeyName.IsNone())
		return;
	
	if (auto BlackboardComponent = GetBlackboardComponent())
		BlackboardComponent->SetValueAsFloat(BlackboardKeys->MoveSpeedBBKey.SelectedKeyName, NewValue);
}
