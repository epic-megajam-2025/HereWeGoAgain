#include "Components/PlayerGestureComponent.h"

#include "Interfaces/GestureCharacter.h"

void UPlayerGestureComponent::OnGestureStarted(bool bDisableMovementDuringGesutre)
{
	Super::OnGestureStarted(bDisableMovementDuringGesutre);
	if (bDisableMovementDuringGesutre)
		GestureCharacter->SetMovementEnabledForGestures(false);
}

void UPlayerGestureComponent::OnGestureFinished()
{
	Super::OnGestureFinished();
	if (bDisableMovementDuringGesture)
		GestureCharacter->SetMovementEnabledForGestures(true);
}