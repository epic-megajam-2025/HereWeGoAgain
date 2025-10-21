// Fill out your copyright notice in the Description page of Project Settings.


#include "NavLinkComponent_CatJump.h"

#include "AIController.h"
#include "AI/Characters/Npc.h"
#include "Navigation/PathFollowingComponent.h"

bool UNavLinkComponent_CatJump::OnLinkMoveStarted(class UObject* PathComp, const FVector& DestPoint)
{
	bool Base = Super::OnLinkMoveStarted(PathComp, DestPoint);
	if (!Base)
		return false;

	auto PathFollowingComponent = Cast<UPathFollowingComponent>(PathComp);
	if (!ensure(PathFollowingComponent))
		return false;

	auto AIController = Cast<AAIController>(PathFollowingComponent->GetOwner());
	if (!ensure(AIController))
		return false;

	auto Character = Cast<ANpc>(AIController->GetPawn());
	if (Character == nullptr)
		return false;

	TScriptInterface<INavLinkCustomInterface> NavLink;
	NavLink.SetObject(this);
	NavLink.SetInterface(this);
	Character->UseNavLink_Jump(DestPoint, NavLink);
	return true;
}

void UNavLinkComponent_CatJump::OnLinkMoveFinished(class UObject* PathComp)
{
	Super::OnLinkMoveFinished(PathComp);
}
