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

	if (bActive)
	{
		PointLaser();
		BP_StartPointingLaser();
	}
	else
	{
		// putting laser pointer actor away because AI perception auto successive range seems to NOT age the laser pointer actor after it stopped moving
		// and hence it's never forgotten which introduces errors in AI logic
		// upd: tried it. bad idea. cat doesn't instantly lose sense of the laser pointer due to how BTs work,
		// so for some moment the cat "perceives" the laser pointer high above players head which also looks ugly.
		// guess i just have to handle edge case of losing laser pointer without relying on auto success range feature of AI perception component
		// LaserPointerActor->SetActorLocation(GetOwner()->GetActorLocation() + FVector::UpVector * 10000.f);
		BP_StopPointingLaser();
	}
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

	PointLaserAt(LaserTraceStart, bHit ? HitResult.Location : HitResult.TraceEnd, HitResult.ImpactNormal);
}


void ULaserPointerComponent::PointLaserAt_Implementation(const FVector& PointingFrom, const FVector& PointingAt, const FVector& SurfaceNormal)
{
	// adding extra offset by surface normal for more reliable EQS tests and AI perception
	LaserPointerActor->SetActorLocation(PointingAt + SurfaceNormal * SurfaceOffset);
	if (bDebug_DrawTrace)
		DrawDebugLine(GetWorld(), PointingFrom, PointingAt, FColor::Red, false, 0.0f, 0, 1.0f);
}
