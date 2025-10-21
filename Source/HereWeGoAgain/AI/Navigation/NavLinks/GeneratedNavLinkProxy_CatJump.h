// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/GeneratedNavLinksProxy.h"
#include "GeneratedNavLinkProxy_CatJump.generated.h"

/**
 * 
 */
UCLASS()
class HEREWEGOAGAIN_API UGeneratedNavLinkProxy_CatJump : public UGeneratedNavLinksProxy
{
	GENERATED_BODY()

public:
	virtual bool OnLinkMoveStarted(class UObject* PathComp, const FVector& DestPoint) override;
	virtual void OnLinkMoveFinished(class UObject* PathComp) override;
	virtual UWorld* GetWorld() const override;
};
