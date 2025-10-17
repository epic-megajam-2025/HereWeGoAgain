
#include "AnimNotifies/AnimNotify_GestureItem.h"

#include "Components/BaseGesturesComponent.h"

void UAnimNotify_GestureItem::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	AActor* Owner = MeshComp->GetOwner();
	if (IsValid(Owner) == false)
	{
		return;
	}
	
	if (UBaseGesturesComponent* GestureComponent = Owner->FindComponentByClass<UBaseGesturesComponent>())
	{
		GestureComponent->AttachGestureItem(GestureItemTag, bAttach);
	}
}
