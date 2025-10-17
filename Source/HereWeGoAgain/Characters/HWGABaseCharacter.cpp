// Fill out your copyright notice in the Description page of Project Settings.


#include "HWGABaseCharacter.h"

#include "Components/HWGACharacterMovementComponent.h"
#include "GAS/HWGAAbilitySystemComponent.h"
#include "GAS/Attributes/MovementAttributeSet.h"

AHWGABaseCharacter::AHWGABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UHWGACharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UHWGAAbilitySystemComponent>(TEXT("Ability System Component"));
	MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>(TEXT("Movement Attribute Set"));
}

void AHWGABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(InitializationGameplayEffect))
	{
		auto GEContext = AbilitySystemComponent->MakeEffectContext();
		auto GESpec = AbilitySystemComponent->MakeOutgoingSpec(InitializationGameplayEffect, 1.f, GEContext);
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*GESpec.Data);
	}
}

UMovementAttributeSet* AHWGABaseCharacter::GetMovementAttributeSet() const
{
	return MovementAttributeSet;
}

UAbilitySystemComponent* AHWGABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}