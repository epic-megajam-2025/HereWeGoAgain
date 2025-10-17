#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AnimNotifiy_GestureSectionEnded.generated.h"

UCLASS()
class GESTURES_API UAnimNotifiy_GestureSectionEnded : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
