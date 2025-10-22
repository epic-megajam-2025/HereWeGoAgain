#include "Npc.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "NavLinkCustomInterface.h"
#include "AI/Controllers/HwgaAiController.h"
#include "AI/Data/AiDataTypes.h"
#include "AI/Data/AIGameplayTags.h"
#include "AI/Data/HWGAGAMEPLAYTAGS.h"
#include "Components/AudioComponent.h"
#include "Components/HWGACharacterMovementComponent.h"
#include "Components/NpcGesturesComponent.h"
#include "Game/LogChannels.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/NpcAbilitySystemComponent.h"
#include "GAS/Attributes/AttentionAttributeSet.h"
#include "GAS/Attributes/NpcPerceptionAttributeSet.h"
#include "Navigation/PathFollowingComponent.h"
#include "Settings/AISettings.h"
#include "Sound/SoundCue.h"

ANpc::ANpc(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UNpcAbilitySystemComponent>(AbilitySystemComponentName)
										  .SetDefaultSubobjectClass<UNpcGesturesComponent>(GesturesComponentName))
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetMesh(), FName("head"));
	PerceptionAttributeSet = CreateDefaultSubobject<UNpcPerceptionAttributeSet>(TEXT("PerceptionAttributeSet"));
	AttentionAttributeSet = CreateDefaultSubobject<UAttentionAttributeSet>(TEXT("AttentionAttributeSet"));
	GetCharacterMovement()->bNotifyApex = true;
}

// (@red) man I wish I was not stupid so I could do a proper physics for launch vector
// TODO reconsider
// void ANpc::UseNavLink_Jump(const FVector& Destination, TScriptInterface<INavLinkCustomInterface> NavLink)
// {
// 	ActiveNavLink = NavLink;
// 	FVector ActorLocation = GetActorLocation();
// 	float Angle = 10.f;
// 	float DistanceXY = FVector::Dist2D(ActorLocation, Destination);
// 	float HeightDiff = Destination.Z - ActorLocation.Z;
// 	float Gravity = GetCharacterMovement()->GetGravityZ();
// 	
// 	if (ActorLocation.Z < Destination.Z)
// 	{
// 		float AngleTan = HeightDiff / DistanceXY;
// 		Angle = FMath::Clamp(FMath::RadiansToDegrees(FMath::Atan(AngleTan)), 10.f, 80.f);
// 	}
//
// 	float g = GetCharacterMovement()->GravityScale *9.81f;
// 	float AngleRad = FMath::DegreesToRadians(Angle);
// 	float Distance2dInMeters = DistanceXY / 100.f;
// 	float HeightDiffInMeters = HeightDiff / 100.f;
// 	float TanA = FMath::Tan(AngleRad);
// 	float CosA = FMath::Cos(AngleRad);
// 	float Cos2A = CosA * CosA;
// 	float a = Distance2dInMeters * Distance2dInMeters * g;
// 	float b = 2.f * (HeightDiffInMeters - Distance2dInMeters * TanA) * Cos2A;
// 	float SpeedInMpS = FMath::Sqrt(a / b);
// 	// float SpeedInMpS = FMath::Sqrt((Distance2dInMeters * Distance2dInMeters * g) / 
// 	// 	(2.0f * (Distance2dInMeters * FMath::Tan(AngleRad) - HeightDiffInMeters) * FMath::Cos(AngleRad) * FMath::Cos(AngleRad)));
//
// 	float Speed = SpeedInMpS * 100.f; // convert to meters per second
// 	FVector DirectionXY = (Destination - ActorLocation).GetSafeNormal2D();
// 	FVector LaunchVector = DirectionXY * Speed * FMath::Cos(AngleRad) 
// 						 + FVector::UpVector * Speed * FMath::Sin(AngleRad);
//
// 	UE_VLOG(this, LogAI_NavLink, Verbose, TEXT("Jumping with speed %.2f and angle %.2f"), Speed, Angle);
// 	UE_VLOG_ARROW(this, LogAI_NavLink, Verbose, ActorLocation, ActorLocation + LaunchVector, FColor::Cyan, TEXT("Launch vector"));
// 	LaunchCharacter(LaunchVector, true, true);
// }

void ANpc::UseNavLink_Jump(const FVector& Destination, TScriptInterface<INavLinkCustomInterface> NavLink)
{
	ActiveNavLink = NavLink;
	
	FVector ActorLocation = GetActorLocation();
	float DistanceXY = FVector::Dist2D(ActorLocation, Destination);
	FVector Direction = (Destination - ActorLocation).GetSafeNormal();
	float HeightDiff = Destination.Z - ActorLocation.Z;
	auto AISettings = GetDefault<UAISettings>();

	float SpeedXY = 100.f, SpeedZ = 100.f;
	
	if (auto SpeedDependencyXY = AISettings->NavLinkJumpSpeedDependencyCurve_XY.GetRichCurveConst())
		SpeedXY = SpeedDependencyXY->Eval(DistanceXY);

	if (auto SpeedDependencyZ = AISettings->NavLinkJumpSpeedDependencyCurve_Z.GetRichCurveConst())
		SpeedZ = SpeedDependencyZ->Eval(HeightDiff);
	
	FVector LaunchVector = FVector(Direction.X * SpeedXY, Direction.Y * SpeedXY, SpeedZ);

	// UE_VLOG(this, LogAI_NavLink, Verbose, TEXT("Jumping with speed %.2f and angle %.2f"), Speed, Angle);
	UE_VLOG(this, LogAI_NavLink, Verbose, TEXT("Using navlink jump, launch vector: %s"), *LaunchVector.ToString());
	UE_VLOG_ARROW(this, LogAI_NavLink, Verbose, ActorLocation, ActorLocation + LaunchVector, FColor::Cyan, TEXT("Launch vector"));
	NavLinkDestination = Destination;
	ChangeGameplayTags(HWGAGameplayTags::Character_State_UsingNavLink, true);
	LaunchCharacter(LaunchVector, true, true);
}

void ANpc::RepositionToNavmesh()
{
	auto NavSystem = UNavigationSystemV1::GetCurrent(this);
	if (NavSystem == nullptr)
		return; // can happen on end game/world destroyed

	const FVector& CurrentActorLocation = GetActorLocation();
	FNavLocation ClosestReachableLocation;
	bool bProjectedNavMesh = NavSystem->ProjectPointToNavigation(CurrentActorLocation, ClosestReachableLocation, FVector(200, 200, 200));
	if (bProjectedNavMesh)
	{
		const bool bNeedToMoveCharacter = FMath::Abs(ClosestReachableLocation.Location.X - CurrentActorLocation.X) > 0.1f
			|| FMath::Abs(ClosestReachableLocation.Location.Y - CurrentActorLocation.Y) > 0.1f; 
		if (bNeedToMoveCharacter)
		{
			UE_VLOG(this, LogHWGA, Verbose, TEXT("Interpolating to a projected nav mesh location"));
			UE_VLOG_LOCATION(this, LogHWGA, Verbose, ClosestReachableLocation.Location, 12.f, FColor::Blue, TEXT("Interpolate to"));
			Cast<UHWGACharacterMovementComponent>(GetCharacterMovement())->InterpolateToLocation(ClosestReachableLocation.Location, 7.5f,
				{ }, false);
		}
		else
		{
			UE_VLOG(this, LogHWGA, Verbose, TEXT("No need to interpolate to navmesh"));
		}
	}
	else
	{
		UE_VLOG(this, LogAI, Warning, TEXT("Failed to project character location to navmesh"));
	}
}

void ANpc::UpdateSenseSettings(const FOnAttributeChangeData& Data)
{
	if (!AIController.IsValid())
		return;
	
	if (Data.Attribute == UNpcPerceptionAttributeSet::GetSightRadiusAttribute())
		AIController->SetSightRadius(Data.NewValue);
	else if (Data.Attribute == UNpcPerceptionAttributeSet::GetSightHalfAngleAttribute())
		AIController->SetSightHalfAngle(Data.NewValue);
	else if(Data.Attribute == UNpcPerceptionAttributeSet::GetHearingRadiusAttribute())
		AIController->SetHearingRadius(Data.NewValue);
}

void ANpc::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	TryFinishUsingNavLink();
	RepositionToNavmesh();
}

void ANpc::NotifyJumpApex()
{
	Super::NotifyJumpApex();
	if (ActiveNavLink.GetObject() != nullptr)
		if (auto HwgaCmc = 	Cast<UHWGACharacterMovementComponent>(GetCharacterMovement()))
			HwgaCmc->InterpolateToLocation(NavLinkDestination, JumpNavLinkInterpolationRate, {}, false);
}

void ANpc::TryFinishUsingNavLink()
{
	if (IsValid(ActiveNavLink.GetObject()))
	{
		auto PathFollowingComponent = GetController()->FindComponentByClass<UPathFollowingComponent>();
		auto CurrentNavLink = Cast<INavLinkCustomInterface>(PathFollowingComponent->GetCurrentCustomLinkOb());
		PathFollowingComponent->FinishUsingCustomLink(CurrentNavLink);
		ChangeGameplayTags(HWGAGameplayTags::Character_State_UsingNavLink, false);
		ActiveNavLink = nullptr;
		NavLinkDestination = FAISystem::InvalidLocation;
	}
}

void ANpc::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	if (PrevMovementMode == MOVE_Custom && PreviousCustomMode == (uint8)EHWGAMovementMode::Interpolating)
	{
		TryFinishUsingNavLink();	
	}
}

void ANpc::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	AIController = Cast<AHwgaAiController>(NewController);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UNpcPerceptionAttributeSet::GetHearingRadiusAttribute())
		.AddUObject(this, &ANpc::UpdateSenseSettings);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UNpcPerceptionAttributeSet::GetSightRadiusAttribute())
		.AddUObject(this, &ANpc::UpdateSenseSettings);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UNpcPerceptionAttributeSet::GetSightHalfAngleAttribute())
		.AddUObject(this, &ANpc::UpdateSenseSettings);
	AIController->SetSightHalfAngle(PerceptionAttributeSet->GetSightHalfAngle());
	AIController->SetSightRadius(PerceptionAttributeSet->GetSightRadius());
	AIController->SetHearingRadius(PerceptionAttributeSet->GetHearingRadius());
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
