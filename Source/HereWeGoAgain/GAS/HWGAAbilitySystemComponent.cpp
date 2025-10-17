// Fill out your copyright notice in the Description page of Project Settings.


#include "HWGAAbilitySystemComponent.h"

#include "AI/Characters/Cat.h"
#include "Attributes/MovementAttributeSet.h"
#include "Components/HWGACharacterMovementComponent.h"


UHWGAAbilitySystemComponent::UHWGAAbilitySystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UHWGAAbilitySystemComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<AHWGABaseCharacter>(GetOwner());
	if (CharacterOwner == nullptr)
		return;
	
	UMovementAttributeSet* MovementAttributeSet = CharacterOwner->GetMovementAttributeSet();
	GetGameplayAttributeValueChangeDelegate(MovementAttributeSet->GetMoveSpeedAttribute()).AddUObject(this, &UHWGAAbilitySystemComponent::OnMoveSpeedChanged);
	GetGameplayAttributeValueChangeDelegate(MovementAttributeSet->GetTurnRateAttribute()).AddUObject(this, &UHWGAAbilitySystemComponent::OnTurnRateChanged);
}

void UHWGAAbilitySystemComponent::OnMoveSpeedChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	CharacterOwner->FindComponentByClass<UHWGACharacterMovementComponent>()->SetWalkSpeed(OnAttributeChangeData.NewValue);	
}

void UHWGAAbilitySystemComponent::OnTurnRateChanged(const FOnAttributeChangeData& OnAttributeChangeData)
{
	CharacterOwner->FindComponentByClass<UHWGACharacterMovementComponent>()->SetTurnRate(OnAttributeChangeData.NewValue);	
}
