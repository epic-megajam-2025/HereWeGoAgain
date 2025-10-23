// 


#include "UpdateSpeedToCatchTargetMMC.h"

#include "AI/Components/NpcComponent.h"

float UUpdateSpeedToCatchTargetMMC::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	auto InstigatorActor = Cast<APawn>(Spec.GetEffectContext().GetInstigator());
	if (InstigatorActor == nullptr)
		return 0.f;
	
	auto InstigatorController = InstigatorActor->GetController();
	if (InstigatorController == nullptr)
		return 0.f;
	
	auto NpcComponent = InstigatorActor->GetController()->FindComponentByClass<UNpcComponent>();
	if (NpcComponent == nullptr)
		return 0.f;

	constexpr float FallbackSpeed = 175.f;
	const FVector& CatchUpTargetLocation = NpcComponent->GetTargetLocation();
	if (CatchUpTargetLocation == FAISystem::InvalidLocation)
		return FallbackSpeed;
	
	if (auto SpeedDependency = NpcComponent->GetCatchUpSpeedDependencyCurve())
	{
		float PredictionDistance = (InstigatorActor->GetActorLocation() - CatchUpTargetLocation).Size() + NpcComponent->GetTargetSpeed() * Spec.GetPeriod();
		return SpeedDependency->Eval(PredictionDistance);
	}
	
	return FallbackSpeed;
}
