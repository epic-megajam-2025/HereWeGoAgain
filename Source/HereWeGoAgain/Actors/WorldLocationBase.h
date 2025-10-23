// 

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/NpcZone.h"
#include "WorldLocationBase.generated.h"

UCLASS(Abstract, NotBlueprintable)
class HEREWEGOAGAIN_API AWorldLocationBase : public AActor, public INpcZone
{
	GENERATED_BODY()

protected:
	virtual float GetEQSStartZCoordinate(const UShapeComponent* ForVolume) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpaceBetweenPointsX = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpaceBetweenPointsY = 200.f;
	
public: // INpcZone
	virtual TArray<FNavLocation> ProvideEqsPoints(const float Density, const float ExtentScale) override;
	virtual bool IsLocationWithinNpcArea(const FVector& TestLocation, float AreaExtent) override;
	virtual FVector GetNpcNavigationLocation(const FVector& QuerierLocation) const override;
};
