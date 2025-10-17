#pragma once

#include "AiDataTypes.generated.h"

#define AI_BRAINMESSAGE_FLAG_IMMEDIATE 0x01

UENUM()
enum class ENpcTargetDistanceEvaluation : uint8
{
	TargetIsStationary,
	TargetIsApproaching,
	TargetIsGettingAway,
};