// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Interfaces/NpcInterface.h"
#include "Characters/HWGABaseCharacter.h"
#include "Npc.generated.h"

class UMovementAttributeSet;

UCLASS()
class HEREWEGOAGAIN_API ANpc : public AHWGABaseCharacter, public INpcInterface
{
	GENERATED_BODY()

public:
	ANpc(const FObjectInitializer& ObjectInitializer);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, TSoftObjectPtr<USoundCue>> Phrases;

	virtual void PossessedBy(AController* NewController) override;
	
private:
	void SendAIMessage(const FGameplayTag& MessageTag, bool bSuccess, bool bImmediately) const;
	
	TWeakObjectPtr<AAIController> AIController;

// interfaces
	
public: // INpc
	virtual void Say(const FGameplayTag& Phrase) override;
	virtual bool PerformNpcGesture(const FGameplayTag& GestureTag) override;
	virtual void StopNpcGesture() override;

public: // IGestureCharacter
	virtual void GestureFinished(bool bSuccess) override;
};
