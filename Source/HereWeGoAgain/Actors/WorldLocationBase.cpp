// 


#include "WorldLocationBase.h"

#include "Components/BoxComponent.h"
#include "Game/LogChannels.h"

TArray<FNavLocation> AWorldLocationBase::ProvideEqsPoints(const float Density, const float ExtentScale)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(AWorldLocationBase::ProvideEqsPoints)
	
	auto WorldLocationVolumes = INpcZone::Execute_GetNpcZoneVolumes(this);
	TArray<FNavLocation> Result;
	
	for (const auto* ZoneVolume : WorldLocationVolumes)
	{
		// TODO handle other volume types
		const UBoxComponent* BoxComponent = Cast<const UBoxComponent>(ZoneVolume);
		if (!ensure(BoxComponent))
			continue;
		
		FVector GuardZoneExtent = BoxComponent->GetScaledBoxExtent();
		FVector Location = GetActorLocation();
		FVector ForwardVector = GetActorForwardVector();
		FVector RightVector = GetActorRightVector();

		const float SizeX = GuardZoneExtent.X * 2.f * ExtentScale;
		const float SizeY = GuardZoneExtent.Y * 2.f * ExtentScale;
		const int32 ItemCountX = FMath::Max(2,  FMath::CeilToInt32(SpaceBetweenPointsX * Density));
		const int32 ItemCountY = FMath::Max(2, FMath::CeilToInt32(SpaceBetweenPointsY * Density));
		
		const float SpaceBetweenX = SizeX / ItemCountX;
		const float SpaceBetweenY = SizeY / ItemCountY;

		Result.Reserve(Result.Num() + ItemCountX + ItemCountY);

		FVector BottomLeftCorner = Location - ForwardVector * SizeX / 2.f - RightVector * SizeY / 2.f;
		
		for (int32 IndexX = 0; IndexX <= ItemCountX; ++IndexX)
		{
			for (int32 IndexY = 0; IndexY <= ItemCountY; ++IndexY)
			{
				FVector ItemLocation = BottomLeftCorner
					+ ForwardVector * SpaceBetweenX * IndexX
					+ RightVector * SpaceBetweenY * IndexY;
				const FNavLocation TestPoint = FNavLocation(ItemLocation);
				Result.Add(TestPoint);
			}
		}	
	}

	return Result;
}

bool AWorldLocationBase::IsLocationWithinNpcArea(const FVector& TestLocation, float AreaExtent)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(ANpcZone::AWorldLocationBase)

	auto NpcVolumes = INpcZone::Execute_GetNpcZoneVolumes(this);
	for (const auto& MarkupVolumeActorComponent : NpcVolumes)
	{
		auto BoxComponent = Cast<UBoxComponent>(MarkupVolumeActorComponent);
		if (!BoxComponent)
		{
			UE_VLOG(this, LogHWGA, Warning, TEXT("IsLocationWithinNpcArea: only box components supported"));
			continue;	
		}
		
		const FVector LocalPoint = BoxComponent->GetComponentTransform().InverseTransformPosition(TestLocation);
		const FVector BoxExtent = BoxComponent->GetScaledBoxExtent() + FVector(AreaExtent);
		if (FMath::Abs(LocalPoint.X) <= BoxExtent.X &&
			FMath::Abs(LocalPoint.Y) <= BoxExtent.Y &&
			FMath::Abs(LocalPoint.Z) <= BoxExtent.Z)
		{
			UE_VLOG_LOCATION(this, LogHWGA, VeryVerbose, TestLocation, 25, FColor::Emerald, TEXT("Within NPC area"));
			return true;
		}
	}

	UE_VLOG_LOCATION(this, LogHWGA, VeryVerbose, TestLocation, 25, FColor::Red, TEXT("Out of NPC area"));
	return false;
}

FVector AWorldLocationBase::GetNpcNavigationLocation(const FVector& QuerierLocation) const
{
	return GetOwner()->GetActorLocation();
}

float AWorldLocationBase::GetEQSStartZCoordinate(const UShapeComponent* ForVolume) const
{
	return GetOwner()->GetActorLocation().Z;
}
