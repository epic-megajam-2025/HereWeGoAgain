// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneratedNavLinkProxy_CatJump.h"

#include "AIController.h"
#include "AI/Characters/Npc.h"
#include "Engine/GameEngine.h"
#include "Navigation/PathFollowingComponent.h"

bool UGeneratedNavLinkProxy_CatJump::OnLinkMoveStarted(class UObject* PathComp, const FVector& DestPoint)
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

void UGeneratedNavLinkProxy_CatJump::OnLinkMoveFinished(class UObject* PathComp)
{
	Super::OnLinkMoveFinished(PathComp);
}

UWorld* UGeneratedNavLinkProxy_CatJump::GetWorld() const
{
#if WITH_EDITOR
	if (GEngine)
	{
		auto World = GEngine->GetWorld();
		if (World)
			return World;
	}
#endif // WITH_EDITOR

	if (UGameEngine* GameEngine = Cast<UGameEngine>(GEngine))
		return GameEngine->GetGameWorld();

	if (UObject* Outer = GetOuter())
		return Outer->GetWorld();

	return nullptr;
}
