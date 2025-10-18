// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"

#include "HWGAAbilitySystemComponent.generated.h"


class AHWGABaseCharacter;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UHWGAAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHWGAAbilitySystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnMoveSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData);
	
	TWeakObjectPtr<AHWGABaseCharacter> CharacterOwner;

private:
	void OnTurnRateChanged(const FOnAttributeChangeData& OnAttributeChangeData);
};
