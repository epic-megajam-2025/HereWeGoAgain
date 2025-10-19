// Fill out your copyright notice in the Description page of Project Settings.


#include "NpcBlackboardKeysDataAsset.h"

#include "BlackboardKeyType_GameplayTag.h"
#include "BehaviorTree/BlackboardData.h"

UBlackboardData* UNpcBlackboardKeysDataAsset::GetBlackboardAsset() const
{
	return BlackboardAsset.IsNull() ? nullptr : BlackboardAsset.LoadSynchronous();
}

UNpcBlackboardKeysDataAsset::UNpcBlackboardKeysDataAsset()
{
	StartLocationBBKey.AllowNoneAsValue(true);
	StartLocationBBKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UNpcBlackboardKeysDataAsset, StartLocationBBKey));

	EndLocationBBKey.AllowNoneAsValue(true);
	EndLocationBBKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UNpcBlackboardKeysDataAsset, EndLocationBBKey));

	MoveSpeedBBKey.AllowNoneAsValue(true);
	MoveSpeedBBKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(UNpcBlackboardKeysDataAsset, MoveSpeedBBKey));

	ActiveBehaviorEvaluatorsTagsBBKey.AllowNoneAsValue(true);
	ActiveBehaviorEvaluatorsTagsBBKey.AllowedTypes.Add(NewObject<UBlackboardKeyType_GameplayTag>(this, GET_MEMBER_NAME_CHECKED(UNpcBlackboardKeysDataAsset, ActiveBehaviorEvaluatorsTagsBBKey)));

}