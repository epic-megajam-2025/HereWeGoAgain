#include "AIGameplayTags.h"

namespace AIGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Ability_Rotate_Event_Activate, "AI.Ability.Rotate.Event.Activate", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Ability_Rotate_Event_Abort, "AI.Ability.Rotate.Event.Abort", "");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Noise_Footstep, "AI.Noise.Footstep", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Noise_Report_VisualContact_Acquired, "AI.Noise.Report.VisualContact.Acquired", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_Noise_Report_VisualContact_Lost, "AI.Noise.Report.VisualContact.Lost", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(GameplayEvent_Npc_Death, "AI.GameplayEvent.Npc.Death", "");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_BrainMessage_Ability_ActivationFailed_CantAfford, "AI.BrainMessage.Ability.ActivationFailed.CantAffordCost", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_BrainMessage_Ability_ActivationFailed_ConditionsNotMet, "AI.BrainMessage.Ability.ActivationFailed.ConditionsNotMet", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_BrainMessage_Gesture_Completed, "AI.BrainMessage.Gesture.Complete", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_BrainMessage_LookAt_Completed, "AI.BrainMessage.LookAt.Complete", "");

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_BrainMessage_SmartObjectInteraction_Completed, "AI.BrainMessage.SmartObjectInteraction.Completed", "");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_SetByCaller_UpdateSpeedToBeAtLocationMMC_Time, "AI.SetByCaller.UpdateSpeedToBeAtLocationMMC.Time", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_SetByCaller_UpdateSpeedToBeAtLocationMMC_MinSpeed, "AI.SetByCaller.UpdateSpeedToBeAtLocationMMC.MinSpeed", "");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(AI_SetByCaller_UpdateSpeedToBeAtLocationMMC_MaxSpeed, "AI.SetByCaller.UpdateSpeedToBeAtLocationMMC.MaxSpeed", "");
}