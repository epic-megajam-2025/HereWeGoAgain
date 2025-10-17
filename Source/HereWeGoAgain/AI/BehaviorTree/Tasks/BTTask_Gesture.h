#pragma once

#include "CoreMinimal.h"
#include "BTTask_HandleGameplayAbility.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Gesture.generated.h"

UCLASS()
class HEREWEGOAGAIN_API UBTTask_Gesture : public UBTTask_HandleGameplayAbility
{
	GENERATED_BODY()

public:
	UBTTask_Gesture();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual FString GetStaticDescription() const override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bAwaitCompletion = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag GestureActionTag;
};
