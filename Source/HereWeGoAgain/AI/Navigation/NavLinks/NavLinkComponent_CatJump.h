// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavLinkCustomComponent.h"
#include "NavLinkComponent_CatJump.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API UNavLinkComponent_CatJump : public UNavLinkCustomComponent
{
	GENERATED_BODY()

public:
	virtual bool OnLinkMoveStarted(class UObject* PathComp, const FVector& DestPoint) override;
	virtual void OnLinkMoveFinished(class UObject* PathComp) override;
};
