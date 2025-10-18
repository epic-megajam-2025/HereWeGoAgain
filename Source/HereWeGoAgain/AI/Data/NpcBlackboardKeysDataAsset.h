// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/BehaviorTree/Decorators/BTDecorator_StoreTaggedBlackboardData.h"
#include "BehaviorTree/BlackboardAssetProvider.h"
#include "Engine/DataAsset.h"
#include "NpcBlackboardKeysDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UNpcBlackboardKeysDataAsset : public UDataAsset, public IBlackboardAssetProvider
{
	GENERATED_BODY()

public:
	UNpcBlackboardKeysDataAsset();
	virtual UBlackboardData* GetBlackboardAsset() const override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UBlackboardData> BlackboardAsset;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBlackboardKeySelector StartLocationBBKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBlackboardKeySelector EndLocationBBKey;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FBlackboardKeySelector MoveSpeedBBKey;
};
