#include "Npc.h"

#include "AIController.h"
#include "AI/Data/AiDataTypes.h"
#include "AI/Data/AIGameplayTags.h"
#include "Components/AudioComponent.h"
#include "Components/NpcGesturesComponent.h"
#include "Game/LogChannels.h"
#include "GAS/NpcAbilitySystemComponent.h"
#include "Sound/SoundCue.h"

ANpc::ANpc(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNpcAbilitySystemComponent>(AbilitySystemComponentName)
										  .SetDefaultSubobjectClass<UNpcGesturesComponent>(GesturesComponentName))
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetMesh(), FName("head"));
}

void ANpc::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AIController = Cast<AAIController>(NewController);
}

void ANpc::Say(const FGameplayTag& Phrase)
{
	auto PhraseSoundSoftPtr = Phrases.Find(Phrase);
	if (PhraseSoundSoftPtr == nullptr || PhraseSoundSoftPtr->IsNull())
	{
		UE_LOG(LogHWGA, Warning, TEXT("Attempt to say phrase %s but there's no such phrase"), *Phrase.ToString())
		return;
	}

	AudioComponent->SetSound(PhraseSoundSoftPtr->LoadSynchronous());
	AudioComponent->Play();
}

bool ANpc::PerformNpcGesture(const FGameplayTag& GestureTag)
{
	return Gesture(GestureTag);
}

void ANpc::StopNpcGesture()
{
	StopGesture();
}

void ANpc::GestureFinished(bool bSuccess)
{
	Super::GestureFinished(bSuccess);
	SendAIMessage(AIGameplayTags::AI_BrainMessage_Gesture_Completed, bSuccess, false);
}

void ANpc::SendAIMessage(const FGameplayTag& MessageTag, bool bSuccess, bool bImmediately) const
{
	if (!AIController.IsValid())
		return;
	
	FAIMessage AIMessage;
	AIMessage.Status = bSuccess ? FAIMessage::Success : FAIMessage::Failure;
	AIMessage.MessageName = MessageTag.GetTagName();
	if (bImmediately)
		AIMessage.MessageFlags |= AI_BRAINMESSAGE_FLAG_IMMEDIATE;

	AIController->GetBrainComponent()->HandleMessage(AIMessage);
}
