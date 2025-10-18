// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GameplayEffectTypes.h"
#include "Characters/HWGABaseCharacter.h"
#include "HwgaAiController.generated.h"

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
	
	void OnMoveSpeedChanged(float NewValue);

protected:
	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcStatesComponent* NpcStatesComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UNpcComponent* NpcComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UNpcBlackboardKeysDataAsset* BlackboardKeys;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UBehaviorTree* RootBehaviorTree;

private:
	TWeakObjectPtr<AHWGABaseCharacter> PossesedCharacter;
};
