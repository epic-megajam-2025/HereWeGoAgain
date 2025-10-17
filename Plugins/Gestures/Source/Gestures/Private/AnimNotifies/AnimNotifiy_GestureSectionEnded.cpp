#include "AnimNotifies/AnimNotifiy_GestureSectionEnded.h"

#include "Components/BaseGesturesComponent.h"

void UAnimNotifiy_GestureSectionEnded::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}
	
	UBaseGesturesComponent* GestureComponent = StaticCast<UBaseGesturesComponent*>(Owner->GetComponentByClass(UBaseGesturesComponent::StaticClass()));
	if (IsValid(GestureComponent))
	{
		GestureComponent->OnMontageSectionEnded();		
	}
	else
	{
#if !UE_BUILD_SHIPPING
		FString LogMessage = FString::Printf(
			TEXT("Trying to notify gesture section ended on an actor that doesn't have UBaseGesturesComponent for animation %s and owner %s"),
			*Animation->GetName(), *MeshComp->GetOwner()->GetName());
		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
#endif
	}
}
