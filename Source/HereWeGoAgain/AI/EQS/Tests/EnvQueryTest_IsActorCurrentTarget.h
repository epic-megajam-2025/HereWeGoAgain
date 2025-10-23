// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "EnvQueryTest_IsActorCurrentTarget.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UEnvQueryTest_IsActorCurrentTarget : public UEnvQueryTest
{
	GENERATED_BODY()

public:
	UEnvQueryTest_IsActorCurrentTarget();
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
};
