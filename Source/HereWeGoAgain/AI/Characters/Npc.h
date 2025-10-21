// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Interfaces/NpcInterface.h"
#include "Characters/HWGABaseCharacter.h"
#include "Npc.generated.h"

class INavLinkCustomInterface;
class UMovementAttributeSet;

UCLASS()
class HEREWEGOAGAIN_API ANpc : public AHWGABaseCharacter, public INpcInterface
{
	GENERATED_BODY()

public:
	ANpc(const FObjectInitializer& ObjectInitializer);
	void UseNavLink_Jump(const FVector& Destination, TScriptInterface<INavLinkCustomInterface> NavLink);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, TSoftObjectPtr<USoundCue>> Phrases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float JumpNavLinkInterpolationRate = 10.f;

	virtual void PossessedBy(AController* NewController) override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void NotifyJumpApex() override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;
	
private:
	void SendAIMessage(const FGameplayTag& MessageTag, bool bSuccess, bool bImmediately) const;
	void TryFinishUsingNavLink();
	void RepositionToNavmesh();

	TWeakObjectPtr<AAIController> AIController;

	UPROPERTY()
	TScriptInterface<INavLinkCustomInterface> ActiveNavLink;

	FVector NavLinkDestination = FAISystem::InvalidLocation;
	
// interfaces
	
public: // INpc
	virtual void Say(const FGameplayTag& Phrase) override;
	virtual bool PerformNpcGesture(const FGameplayTag& GestureTag) override;
	virtual void StopNpcGesture() override;

public: // IGestureCharacter
	virtual void GestureFinished(bool bSuccess) override;
};
