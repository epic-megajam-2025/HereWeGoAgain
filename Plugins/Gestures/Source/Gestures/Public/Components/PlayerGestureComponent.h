#pragma once

#include "CoreMinimal.h"
#include "BaseGesturesComponent.h"
#include "PlayerGestureComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GESTURES_API UPlayerGestureComponent : public UBaseGesturesComponent
{
	GENERATED_BODY()

protected:
	virtual void OnGestureStarted(bool bDisableMovementDuringGesutre) override;
	virtual void OnGestureFinished() override;
};
