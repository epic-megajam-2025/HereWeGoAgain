// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "HWGABaseCharacter.generated.h"

class UGameplayEffect;
class UMovementAttributeSet;

UCLASS()
class HEREWEGOAGAIN_API AHWGABaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AHWGABaseCharacter(const FObjectInitializer& ObjectInitializer);
	UMovementAttributeSet* GetMovementAttributeSet() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHWGAAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
	UMovementAttributeSet* MovementAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InitializationGameplayEffect;

public: // IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
};
