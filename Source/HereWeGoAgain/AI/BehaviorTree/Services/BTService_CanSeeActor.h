// 

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CanSeeActor.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UBTService_CanSeeActor : public UBTService
{
	GENERATED_BODY()

public:
	UBTService_CanSeeActor();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual FString GetStaticDescription() const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector ActorBBKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector OutCanSeeActorBBKey;

private:
	bool CanSee(UBehaviorTreeComponent& OwnerComp) const;
};
