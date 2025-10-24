// copyright Bitrat

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_LookForActiveLaserPointer.generated.h"

/**
 * temp service. hack to get shit working
 */
UCLASS()
class HEREWEGOAGAIN_API UBTService_LookForActiveLaserPointer : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_LookForActiveLaserPointer();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector OutLaserPointerBBKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AttentionTriggerTag;
};
