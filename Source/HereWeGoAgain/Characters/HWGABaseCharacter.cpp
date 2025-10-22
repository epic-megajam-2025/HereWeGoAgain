// Fill out your copyright notice in the Description page of Project Settings.


#include "HWGABaseCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AI/Data/HWGAGameplayTags.h"
#include "Components/BaseGesturesComponent.h"
#include "Components/HWGACharacterMovementComponent.h"
#include "Data/GestureTags.h"
#include "Game/LogChannels.h"
#include "GAS/GameplayAbilityTargetData_Gesture.h"
#include "GAS/HWGAAbilitySystemComponent.h"
#include "GAS/Attributes/MovementAttributeSet.h"

FName AHWGABaseCharacter::AbilitySystemComponentName(TEXT("Ability System Component"));
FName AHWGABaseCharacter::GesturesComponentName(TEXT("GesturesComponent"));

AHWGABaseCharacter::AHWGABaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UHWGACharacterMovementComponent>(CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
	AbilitySystemComponent = CreateDefaultSubobject<UHWGAAbilitySystemComponent>(AbilitySystemComponentName);
	MovementAttributeSet = CreateDefaultSubobject<UMovementAttributeSet>(TEXT("Movement Attribute Set"));
	GesturesComponent = CreateDefaultSubobject<UBaseGesturesComponent>(GesturesComponentName);
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

	if (!GrantedAbilities.IsEmpty())
	{
		for (const auto& AbilityClassSoftPtr : GrantedAbilities)
		{
			if (AbilityClassSoftPtr.IsNull())
				continue;
			
			auto AbilityClass = AbilityClassSoftPtr.LoadSynchronous(); 
			auto AbilitySpec = AbilitySystemComponent->BuildAbilitySpecFromClass(AbilityClass);
			AbilitySystemComponent->GiveAbility(AbilitySpec);
		}
	}
}

void AHWGABaseCharacter::Falling()
{
	Super::Falling();
	ChangeGameplayTags(HWGAGameplayTags::Character_State_InAir, true);
}

void AHWGABaseCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	ChangeGameplayTags(HWGAGameplayTags::Character_State_InAir, false);
}

UMovementAttributeSet* AHWGABaseCharacter::GetMovementAttributeSet() const
{
	return MovementAttributeSet;
}

void AHWGABaseCharacter::AddGameplayTag(const FGameplayTag& Tag, bool bUnique)
{
	if (bUnique && CharacterTags.HasTag(Tag))
		return;

	ChangeGameplayTags(Tag.GetSingleTagContainer(), true);
}

void AHWGABaseCharacter::RemoveGameplayTag(const FGameplayTag& Tag)
{
	ChangeGameplayTags(Tag.GetSingleTagContainer(), false);
}

void AHWGABaseCharacter::FaceRotation(FRotator NewControlRotation, float DeltaTime)
{
	// deliberately not using Super:: logic, because it doesn't have interpolation
	// Super::FaceRotation(NewControlRotation, DeltaTime);

	// Only if we actually are going to use any component of rotation.
	if (bUseControllerRotationPitch || bUseControllerRotationYaw || bUseControllerRotationRoll)
	{
		const FRotator CurrentRotation = GetActorRotation();

		if (!bUseControllerRotationPitch)
		{
			NewControlRotation.Pitch = CurrentRotation.Pitch;
		}

		if (!bUseControllerRotationYaw)
		{
			NewControlRotation.Yaw = CurrentRotation.Yaw;
		}

		if (!bUseControllerRotationRoll)
		{
			NewControlRotation.Roll = CurrentRotation.Roll;
		}

#if ENABLE_NAN_DIAGNOSTIC
		if (NewControlRotation.ContainsNaN())
		{
			logOrEnsureNanError(TEXT("APawn::FaceRotation about to apply NaN-containing rotation to actor! New:(%s), Current:(%s)"), *NewControlRotation.ToString(), *CurrentRotation.ToString());
		}
#endif

		auto InterpolatedRotation = FMath::RInterpTo(CurrentRotation, NewControlRotation, DeltaTime,
			RotateToInterpolationRate * GetCharacterMovement()->RotationRate.Yaw);
		SetActorRotation(InterpolatedRotation);
	}
}

void AHWGABaseCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
#if WITH_EDITOR
	FVector StartLocation = GetActorLocation() + FVector::UpVector * 35.f;
	UE_VLOG_ARROW(this, LogCharacter_FV, VeryVerbose, StartLocation, StartLocation + GetActorForwardVector() * 150.f, FColor::Blue, TEXT("FV"));
#endif
}

void AHWGABaseCharacter::UpdateStrafing()
{
	bool bRequireStrafing = StrafeWhenCharacterInState.Matches(CharacterTags);
	SetStrafing(bRequireStrafing);
}

void AHWGABaseCharacter::OnGameplayTagsChanged_Implementation()
{
	UpdateStrafing();
}

void AHWGABaseCharacter::SetStrafing_Implementation(bool bRequireStrafing)
{
	auto CMC = GetCharacterMovement();
	CMC->bOrientRotationToMovement = !bRequireStrafing;
	CMC->bUseControllerDesiredRotation = bRequireStrafing;
	bUseControllerRotationYaw = false;
}

void AHWGABaseCharacter::ChangeGameplayTags(const FGameplayTag& ChangedTag, bool bAppend)
{
	if (bAppend)
		CharacterTags.AddTag(ChangedTag);
	else
		CharacterTags.RemoveTag(ChangedTag);

	OnGameplayTagsChanged();
}

void AHWGABaseCharacter::ChangeGameplayTags(const FGameplayTagContainer& DeltaTags, bool bAppend)
{
	if (bAppend)
		CharacterTags.AppendTags(DeltaTags);
	else
		CharacterTags.RemoveTags(DeltaTags);

	OnGameplayTagsChanged();
}

UAbilitySystemComponent* AHWGABaseCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AHWGABaseCharacter::GestureStarted()
{
}

void AHWGABaseCharacter::GestureFinished(bool bSuccess)
{
}

UGesturesDataAsset* AHWGABaseCharacter::GetGestures() const
{
	return !GesturesDataAsset.IsNull() ? GesturesDataAsset.LoadSynchronous() : nullptr;
}

void AHWGABaseCharacter::SetMovementEnabledForGestures(bool bEnabled)
{
	// TODO (@red)
}

void AHWGABaseCharacter::GetGestureCharacterActiveTags(FGameplayTagContainer& OutTags) const
{
	OutTags.AppendTags(CharacterTags);
	OutTags.AppendTags(AbilitySystemComponent->GetOwnedGameplayTags());
}

void AHWGABaseCharacter::SetRightHandGesturePositioning(const FVector& RightHandWorldLocation)
{
}

void AHWGABaseCharacter::SetLeftHandGesturePositioning(const FVector& LeftHandWorldLocation)
{
}

void AHWGABaseCharacter::ResetRightHandGesturePositioning()
{
}

void AHWGABaseCharacter::ResetLeftHandGesturePositioning()
{
}

bool AHWGABaseCharacter::IsGameLoading_Gesture() const
{
	return false;
}

void AHWGABaseCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer.AppendTags(CharacterTags);
	TagContainer.AppendTags(AbilitySystemComponent->GetOwnedGameplayTags());
	TagContainer.AddTag(CharacterId);
}

bool AHWGABaseCharacter::Gesture(const FGameplayTag& GestureTag)
{
	if (!GestureTag.IsValid())
		return false;
	
	FGameplayEventData Payload;
	FGameplayAbilityTargetData_Gesture* TargetData = new FGameplayAbilityTargetData_Gesture();
	TargetData->GestureTag = GestureTag;
	Payload.TargetData.Add(TargetData);
	Payload.Instigator = this;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, GestureGameplayTags::Gesture_Ability_Gesture_Event_Activate, Payload);
	return true;
}

void AHWGABaseCharacter::StopGesture()
{
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, GestureGameplayTags::Gesture_Ability_Gesture_Event_Abort, FGameplayEventData());
}