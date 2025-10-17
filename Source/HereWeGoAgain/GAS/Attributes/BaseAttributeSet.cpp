// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"

#include "GameplayEffectAggregator.h"

/** Custom functions. The idea here is that we may want to mix and match these (if FAggregatorEvaluateMetaData starts to hold more than just the qualifier functions) */
void QualifierFunc_UseAllModifiersButOnlyLatestOverride(const FAggregatorEvaluateParameters& EvalParameters, const FAggregator* Aggregator)
{
	// We want to inhibit all qualified negative effects except for the most severe. We want to leave positive modifiers alone
	const FAggregatorMod* LatestOverrideMod = nullptr;
	Aggregator->ForEachMod( [&](const FAggregatorModInfo& ModInfo)
	{
		switch (ModInfo.Op)
		{
			case EGameplayModOp::Override:
				LatestOverrideMod = ModInfo.Mod;
			break;
			default:
				break;
		}
	});
	
	Aggregator->ForEachMod( [&](const FAggregatorModInfo& ModInfo)
	{
		if (!ModInfo.Mod->Qualifies())
		{
			// Mod doesn't qualify (for other reasons) so ignore
			return;
		}

		switch( ModInfo.Op )
		{
		case EGameplayModOp::Additive:
			break;
		case EGameplayModOp::Multiplicitive:
			break;
		case EGameplayModOp::Division:
			break;
		case EGameplayModOp::Override:
			if (ModInfo.Mod != LatestOverrideMod)
				ModInfo.Mod->SetExplicitQualifies(false);
			
			// ExpectedDelta = ModInfo.Mod->EvaluatedMagnitude - BaseValue;
			break;
		default:
			break;
		};
	});
}

FAggregatorEvaluateMetaData FHWGAAggregatorEvaluateMetaDataLibrary::UseAllModifiersButOnlyLatestOverride(QualifierFunc_UseAllModifiersButOnlyLatestOverride);
