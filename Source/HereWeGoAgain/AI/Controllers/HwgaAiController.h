// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Characters/HWGABaseCharacter.h"
#include "HwgaAiController.generated.h"

class UNpcBehaviorEvaluatorComponent;
class UNpcBlackboardKeysDataAsset;
class UNpcComponent;
class UNpcStatesComponent;

UCLASS()
class HEREWEGOAGAIN_API AHwgaAiController : public AAIController
{
	GENERATED_BODY()

public:
	AHwgaAiController(const FObjectInitializer& ObjectInitializer);

	virtual void SetFocus(AActor* NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay) override;
	virtual void SetFocalPoint(FVector NewFocus, EAIFocusPriority::Type InPriority = EAIFocusPriority::Gameplay) override;
	virtual void ClearFocus(EAIFocusPriority::Type InPriority) override;

	void SetSightRadius(const float NewRadius);
	void SetSightHalfAngle(const float NewHalfAngle);
	void SetHearingRadius(const float NewHearingRadius);
	
	virtual void Tick(float DeltaSeconds) override;
	
protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcStatesComponent* NpcStatesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcComponent* NpcComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcBehaviorEvaluatorComponent* NpcBehaviorEvaluatorComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNpcBlackboardKeysDataAsset* BlackboardKeys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBehaviorTree* RootBehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTagContainer InitiallyActiveBehaviorEvaluators;

private:
	TWeakObjectPtr<AHWGABaseCharacter> PossessedCharacter;
};
