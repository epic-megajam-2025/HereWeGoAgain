#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_GestureItem.generated.h"

UCLASS()
class GESTURES_API UAnimNotify_GestureItem : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag GestureItemTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAttach = true;
};
