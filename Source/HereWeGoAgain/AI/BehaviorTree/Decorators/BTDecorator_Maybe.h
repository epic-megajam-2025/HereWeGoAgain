

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/ValueOrBBKey.h"
#include "BTDecorator_Maybe.generated.h"

UCLASS()
class HEREWEGOAGAIN_API UBTDecorator_Maybe : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_Maybe();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	virtual FString GetStaticDescription() const override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bForceSuccess = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FValueOrBBKey_Float Probability = 0.5f;
	
	virtual void OnNodeProcessed(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type& NodeResult) override;
};
