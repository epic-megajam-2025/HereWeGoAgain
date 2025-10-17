#include "LaserPointerComponent.h"

#include "Actors/AttentionTrigger.h"

ULaserPointerComponent::ULaserPointerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void ULaserPointerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
										   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!LaserPointerActor.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("LaserPointerActor is not valid!"));
		SetComponentTickEnabled(false);
		return;
	}

	PointLaser();
}

void ULaserPointerComponent::SetLaserPointerActive(bool bActive)
{
	bLaserActive = bActive;
	SetComponentTickEnabled(bActive);
	if (LaserPointerActor.IsValid())
		LaserPointerActor->SetAttentionTriggerActive(bActive);
}

void ULaserPointerComponent::SetLaserPointerActor(AAttentionTrigger* NewLaserPointerActor)
{
	LaserPointerActor = NewLaserPointerActor;
}

AAttentionTrigger* ULaserPointerComponent::GetLaserPointerActor() const
{
	return LaserPointerActor.Get();
}

void ULaserPointerComponent::PointLaser()
{
	FVector ViewLocation;
	FRotator ViewRotation;
	GetOwner()->GetActorEyesViewPoint(ViewLocation, ViewRotation);
	FVector LaserTraceStart = ViewLocation; // can be replaced with a socket on a laser pointer mesh
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	
	// TODO (@red) consider async trace if performance is an issue
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, LaserTraceStart, LaserTraceStart + ViewRotation.Vector() * LaserTraceDistance,
		LaserTraceChannel, QueryParams);

	PointLaserAt(LaserTraceStart, bHit ? HitResult.Location : HitResult.TraceEnd);
}


void ULaserPointerComponent::PointLaserAt_Implementation(const FVector& PointingFrom, const FVector& PointingAt)
{
	LaserPointerActor->SetActorLocation(PointingAt);
	if (bDebug_DrawTrace)
		DrawDebugLine(GetWorld(), PointingFrom, PointingAt, FColor::Red, false, 0.0f, 0, 1.0f);
}
