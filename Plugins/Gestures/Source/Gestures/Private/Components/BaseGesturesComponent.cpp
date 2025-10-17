#include "Components/BaseGesturesComponent.h"

#include "Components/CapsuleComponent.h"
#include "Data/GestureDataTypes.h"
#include "Data/GesturesContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/GestureCharacter.h"

DEFINE_LOG_CATEGORY(LogGesture)

UBaseGesturesComponent::UBaseGesturesComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBaseGesturesComponent::BeginPlay()
{
	Super::BeginPlay();
	CharacterOwner = Cast<ACharacter>(GetOwner());
	auto GestureCharacterInterface = Cast<IGestureCharacter>(GetOwner());
	ensure(GestureCharacterInterface);
	GestureCharacter.SetObject(GetOwner());
	GestureCharacter.SetInterface(GestureCharacterInterface);
	CachedGestures = GestureCharacter->GetGestures();
	ensure(IsValid(CachedGestures));
}

void UBaseGesturesComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (const auto& ActiveGestureItem : GestureAttachedItems)
	{
		if (!ActiveGestureItem.Value.LeftHandItemSocketAttachment.IsNone())
		{
			FVector SocketLocation = ActiveGestureItem.Value.AttachedMeshComponent->GetSocketLocation(ActiveGestureItem.Value.LeftHandItemSocketAttachment);
			GestureCharacter->SetLeftHandGesturePositioning(SocketLocation);
		}

		if (!ActiveGestureItem.Value.RightHandItemSocketAttachment.IsNone())
		{
			FVector SocketLocation = ActiveGestureItem.Value.AttachedMeshComponent->GetSocketLocation(ActiveGestureItem.Value.RightHandItemSocketAttachment);
			GestureCharacter->SetRightHandGesturePositioning(SocketLocation);
		}
	}
}

FGestureResult UBaseGesturesComponent::TryStartGesture(const FGameplayTag& GestureTag)
{
	FGestureResult Result;
	
	const TArray<FGesture>* GestureOptions = SelectGestureOptions(GestureTag);
	if (GestureOptions == nullptr || GestureOptions->Num() <= 0)
	{
		return Result;
	}
	
	const int Index = FMath::RandRange(0, GestureOptions->Num() - 1);
	const FGesture& SelectedGesture = (*GestureOptions)[Index];
	if (!ensure(SelectedGesture.GestureMontage))
		return Result;

	ActiveGestureIndex = Index;
	ActiveGestureTag = GestureTag;

	Result.bLooped = SelectedGesture.bLooped;
	Result.ExpectedDuration = CharacterOwner->PlayAnimMontage(SelectedGesture.GestureMontage);
	if (!Result.bLooped)
	{
		GetWorld()->GetTimerManager().SetTimer(NonLoopedGestureTimer, this, &UBaseGesturesComponent::NonLoopedMontageFinished, Result.ExpectedDuration);
	}
		
	if (SelectedGesture.bDisableCollision)
	{
		// IDK for sure but I guess this will help the AI avoidance logic to ignore gesturing character's collision which can be useful in some cases
		// InitialMeshCollisionEnabledType = CharacterOwner->GetMesh()->GetCollisionEnabled();
		InitialCapsuleCollisionEnabledType = CharacterOwner->GetCapsuleComponent()->GetCollisionEnabled();
		// CharacterOwner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	int EndGestureSectionIndex = SelectedGesture.GestureMontage->GetSectionIndex(SelectedGesture.EndLoopMontageSection);
	ActiveGestureEndTime = EndGestureSectionIndex != INDEX_NONE
		? SelectedGesture.GestureMontage->GetSectionLength(EndGestureSectionIndex)
		: 0.f;
	
	OnGestureStarted(SelectedGesture.bDisableMovementDuringGesture);

	Result.bStarted = true;
	return Result;
}

FGestureResult UBaseGesturesComponent::TryStartGesture(const FGameplayTag& GestureTag,
	const TMap<FGameplayTag, FGestureItem>& InExternalGestureItems)
{
	ExternalGestureItems = InExternalGestureItems;
	return TryStartGesture(GestureTag);
}

float UBaseGesturesComponent::TryStopGesture(bool bForce)
{
	if (ActiveGestureIndex < 0 || ActiveGestureTag.IsValid() == false)
	{
		return -1.f;
	}

	FGesture RunningGesture;
	GetActiveGesture(RunningGesture);
	float EndGestureDelay = 0.f;
	UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	if (IsValid(RunningGesture.GestureMontage) && AnimInstance->Montage_IsPlaying(RunningGesture.GestureMontage))
	{
		int32 EndSectionIndex = RunningGesture.GestureMontage->GetSectionIndex(RunningGesture.EndLoopMontageSection);
		if (EndSectionIndex != INDEX_NONE)
		{
			EndGestureDelay = RunningGesture.GestureMontage->GetSectionLength(EndSectionIndex);
			AnimInstance->Montage_JumpToSection(RunningGesture.EndLoopMontageSection, RunningGesture.GestureMontage);
		}
		else
		{
			AnimInstance->Montage_Stop(0.2f, RunningGesture.GestureMontage);
		}

		if (RunningGesture.bDisableCollision)
		{
			// CharacterOwner->GetMesh()->SetCollisionEnabled(InitialMeshCollisionEnabledType);
			CharacterOwner->GetCapsuleComponent()->SetCollisionEnabled(InitialCapsuleCollisionEnabledType);
		}
	}
	
    ActiveGestureTag = FGameplayTag::EmptyTag;
	ActiveGestureIndex = -1;
	ActiveContextGestureIndex = false;
	ActiveGestureEndTime = 0.f;
	
    if (GestureAttachedItems.Num() > 0)
    {
        for (const auto& GestureAttachedItem : GestureAttachedItems)
        {
            GestureAttachedItem.Value.AttachedMeshComponent->DestroyComponent();
        	CleanAttachedGestureItemsHandsPositionings(&GestureAttachedItem.Value);
        }
        
        GestureAttachedItems.Reset();
    }

	GetWorld()->GetTimerManager().ClearTimer(NonLoopedGestureTimer);
	GetWorld()->GetTimerManager().ClearTimer(EndGestureDelayTimer);
	
	if (EndGestureDelay > 0.1f && !bForce)
		GetWorld()->GetTimerManager().SetTimer(EndGestureDelayTimer, this, &UBaseGesturesComponent::OnGestureFinished, EndGestureDelay);
	else
		OnGestureFinished();

    return EndGestureDelay;
}

const FGameplayTag& UBaseGesturesComponent::GetActiveGestureTag() const
{
	return ActiveGestureTag;
}

void UBaseGesturesComponent::CleanAttachedGestureItemsHandsPositionings(const FGestureAttachedItemData* AttachedItem)
{
	if (!AttachedItem->LeftHandItemSocketAttachment.IsNone())
		GestureCharacter->ResetLeftHandGesturePositioning();

	if (!AttachedItem->RightHandItemSocketAttachment.IsNone())
		GestureCharacter->ResetRightHandGesturePositioning();
}

void UBaseGesturesComponent::AttachGestureItem(const FGameplayTag& ItemTag, bool bAttach)
{
    FGesture ActiveGesture;
	if (GetActiveGesture(ActiveGesture) == false)
	{
		return;
	}

    if (bAttach)
    {
    	const FGestureItem* GestureItem = ExternalGestureItems.Find(ItemTag);
    	if (GestureItem == nullptr)
    		GestureItem = ActiveGesture.GestureItems.Find(ItemTag);
    	
        if (GestureItem == nullptr)
	        return;
    	
        UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(CharacterOwner.Get());
        StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
        StaticMeshComponent->RegisterComponent();
        StaticMeshComponent->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, GestureItem->AttachmentSocket);
        StaticMeshComponent->SetStaticMesh(GestureItem->StaticMesh.LoadSynchronous());

        if (auto* ExistingAttachedItem = GestureAttachedItems.Find(ItemTag))
        {
            ExistingAttachedItem->AttachedMeshComponent->DestroyComponent();
        	ExistingAttachedItem->ItemMesh = GestureItem->StaticMesh;
        	ExistingAttachedItem->AttachmentSocket = GestureItem->AttachmentSocket;
            GestureAttachedItems[ItemTag].AttachedMeshComponent = StaticMeshComponent;
        }
        else
        {
        	FGestureAttachedItemData AttachedItemData
        	{
        		StaticMeshComponent, NAME_None, NAME_None,
        		GestureItem->AttachmentSocket, GestureItem->StaticMesh
        	};
            GestureAttachedItems.Add(ItemTag, AttachedItemData);
        }

    	if (!GestureItem->LeftHandPositioningSocketName.IsNone() && GestureItem->LeftHandPositioningSocketName != GestureItem->AttachmentSocket)
    	{
    		GestureAttachedItems[ItemTag].LeftHandItemSocketAttachment = GestureItem->LeftHandPositioningSocketName;
    		SetComponentTickEnabled(true);
    	}

    	if (!GestureItem->RightHandPositioningSocketName.IsNone() && GestureItem->RightHandPositioningSocketName != GestureItem->AttachmentSocket)
    	{
    		GestureAttachedItems[ItemTag].RightHandItemSocketAttachment = GestureItem->RightHandPositioningSocketName;
    		SetComponentTickEnabled(true);
    	}
    }
    else
    {
	    if (auto* AttachedItem = GestureAttachedItems.Find(ItemTag))
    	{
    		AttachedItem->AttachedMeshComponent->DestroyComponent();
	    	CleanAttachedGestureItemsHandsPositionings(AttachedItem);
    		GestureAttachedItems.Remove(ItemTag);
    	}
    }
}

void UBaseGesturesComponent::OnMontageSectionEnded() const
{
	FGesture ActiveGesture;
	if (GetActiveGesture(ActiveGesture) == false || ActiveGesture.IntermediateSectionsWeights.Num() <= 0)
	{
		return;
	}
	
	UAnimInstance* AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	const FName& CurrentSectionName = AnimInstance->Montage_GetCurrentSection(ActiveGesture.GestureMontage);

	float TotalWeight = ActiveGesture.MainLoopWeight;
	for (const auto& Section : ActiveGesture.IntermediateSectionsWeights)
	{
		TotalWeight += Section.Value;
	}

	float RandomWeight = FMath::RandRange(0.f, TotalWeight);
	if (RandomWeight <= ActiveGesture.MainLoopWeight)
	{
		if (CurrentSectionName != ActiveGesture.MainLoopMontageSection)
		{
			// AnimInstance->Montage_SetNextSection(CurrentSectionName, ActiveGesture.MainLoopMontageSection, ActiveGesture.GestureMontage);
			AnimInstance->Montage_JumpToSection(ActiveGesture.MainLoopMontageSection, ActiveGesture.GestureMontage);
		}
		return;
	}

	RandomWeight -= ActiveGesture.MainLoopWeight;
	for (const auto& Section : ActiveGesture.IntermediateSectionsWeights)
	{
		if (RandomWeight <= Section.Value)
		{
			// AnimInstance->Montage_SetNextSection(CurrentSectionName, Section.Key, ActiveGesture.GestureMontage);
			AnimInstance->Montage_JumpToSection(Section.Key, ActiveGesture.GestureMontage);
			break;
		}

		RandomWeight -= Section.Value;
	}
}

void UBaseGesturesComponent::OnGestureStarted(bool bDisableMovementDuringGesture_In)
{
	GestureCharacter->GestureStarted();
	bDisableMovementDuringGesture = bDisableMovementDuringGesture_In;
}

void UBaseGesturesComponent::OnGestureFinished()
{
	GestureCharacter->GestureFinished(true);
	OnGestureFinishedEvent.Broadcast();
	ExternalGestureItems.Empty();
	SetComponentTickEnabled(false);
}

const TArray<FGesture>* UBaseGesturesComponent::SelectGestureOptions(const FGameplayTag& GestureTag)
{
	auto GestureOptions = CachedGestures->GesturesOptions.Find(GestureTag);
	if (!GestureOptions)
		return nullptr;
	
	FGameplayTagContainer CharacterTags;
	GestureCharacter->GetGestureCharacterActiveTags(CharacterTags);
	for (int i = 0; i < GestureOptions->ContextGestures.Num(); i++)
	{
		if (!GestureOptions->ContextGestures[i].CharacterState.IsEmpty() && GestureOptions->ContextGestures[i].CharacterState.Matches(CharacterTags))
		{
			ActiveContextGestureIndex = i;
			return &GestureOptions->ContextGestures[i].Gestures;
		}
	}

	ActiveContextGestureIndex = -1;
	return &CachedGestures->GesturesOptions.Find(GestureTag)->Gestures;
}

bool UBaseGesturesComponent::GetActiveGesture(FGesture& OutGesture) const
{
	if (ActiveGestureIndex < 0 || ActiveGestureTag.IsValid() == false)
		return false;
	
	if (!IsValid(CachedGestures))
		return false;
	
	OutGesture = ActiveContextGestureIndex >= 0
		? CachedGestures->GesturesOptions[ActiveGestureTag].ContextGestures[ActiveContextGestureIndex].Gestures[ActiveGestureIndex]
		: CachedGestures->GesturesOptions[ActiveGestureTag].Gestures[ActiveGestureIndex];
	
	return true;
}

float UBaseGesturesComponent::GetActiveGestureEndDuration() const
{
	return ActiveGestureEndTime;
}

void UBaseGesturesComponent::OnGameSaved()
{
	for (auto& AttachedItem : GestureAttachedItems)
		AttachedItem.Value.ItemMesh = AttachedItem.Value.AttachedMeshComponent->GetStaticMesh();

	if (auto World = GetWorld())
	{
		auto& TimerManager = World->GetTimerManager();
		if (NonLoopedGestureTimer.IsValid() && TimerManager.IsTimerActive(NonLoopedGestureTimer))
			NonLoopedGestureRemainingTime = TimerManager.GetTimerRemaining(NonLoopedGestureTimer);

		if (EndGestureDelayTimer.IsValid() && TimerManager.IsTimerActive(EndGestureDelayTimer))
			EndGestureDelayRemainingTime = TimerManager.GetTimerRemaining(EndGestureDelayTimer);
	}
}

void UBaseGesturesComponent::OnGameLoaded()
{
	for (auto& AttachedItem : GestureAttachedItems)
	{
		UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(CharacterOwner.Get());
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		StaticMeshComponent->RegisterComponent();
		StaticMeshComponent->AttachToComponent(CharacterOwner->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, AttachedItem.Value.AttachmentSocket);
		StaticMeshComponent->SetStaticMesh(AttachedItem.Value.ItemMesh.LoadSynchronous());
	}

	auto& TimerManager = GetWorld()->GetTimerManager();
	if (NonLoopedGestureRemainingTime > 0.f)
		TimerManager.SetTimer(NonLoopedGestureTimer, this, &UBaseGesturesComponent::NonLoopedMontageFinished, NonLoopedGestureRemainingTime);

	if (EndGestureDelayRemainingTime > 0.f)
		TimerManager.SetTimer(EndGestureDelayTimer, this, &UBaseGesturesComponent::OnGestureFinished, EndGestureDelayRemainingTime);

	OnGestureStarted(bDisableMovementDuringGesture);
}

void UBaseGesturesComponent::NonLoopedMontageFinished()
{
	TryStopGesture(true);
}
