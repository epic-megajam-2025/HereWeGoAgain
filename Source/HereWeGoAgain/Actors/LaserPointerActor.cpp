// Fill out your copyright notice in the Description page of Project Settings.


#include "LaserPointerActor.h"

ALaserPointerActor::ALaserPointerActor()
{
	// for (int i = -3; i < 3; i++)
	// 	for (int j = -3; j < 3; j++)
	// 		for (int k = 0; k < 3; k++)
	// 			VisibilityChecksOffsets.Add(FVector(i * 15.f, j * 15.f, k * 15.f));
}

UAISense_Sight::EVisibilityResult ALaserPointerActor::CanBeSeenFrom(const FCanBeSeenFromContext& Context,
                                                                    FVector& OutSeenLocation, int32& OutNumberOfLoSChecksPerformed, int32& OutNumberOfAsyncLosCheckRequested,
                                                                    float& OutSightStrength, int32* UserData, const FOnPendingVisibilityQueryProcessedDelegate* Delegate)
{
	if (!bAttentionTriggerActive)
	{
		OutSightStrength = 0.f;
		return UAISense_Sight::EVisibilityResult::NotVisible;
	}
	
	FVector Base = GetActorLocation();
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Context.IgnoreActor);
	Params.AddIgnoredActor(this);
	for (int i = 0; i < VisibilityChecksOffsets.Num(); i++)
	{
		FHitResult HitResult;
		FVector TraceEnd = Base + VisibilityChecksOffsets[i];
		bool bTraceBlocked = GetWorld()->LineTraceSingleByChannel(HitResult, Context.ObserverLocation, TraceEnd, ECC_Visibility, Params);
		OutNumberOfLoSChecksPerformed++;
		if (!bTraceBlocked)
		{
			OutSeenLocation = TraceEnd;
			OutSightStrength = 1.f;
			return UAISense_Sight::EVisibilityResult::Visible;
		}
	}

	return UAISense_Sight::EVisibilityResult::NotVisible;
}
