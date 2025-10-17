// Copyright Epic Games, Inc. All Rights Reserved.


#include "HWGAPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "HereWeGoAgainCameraManager.h"
#include "Actors/AttentionTrigger.h"
#include "Components/LaserPointerComponent.h"
#include "Game/LogChannels.h"

AHWGAPlayerController::AHWGAPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AHereWeGoAgainCameraManager::StaticClass();
}

void AHWGAPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (LaserPointerActor == nullptr)
	{
		LaserPointerActor = GetWorld()->SpawnActor<AAttentionTrigger>(LaserPointerWidgetClass, InPawn->GetActorLocation(), InPawn->GetActorRotation());
		auto LaserPointerComponent = InPawn->FindComponentByClass<ULaserPointerComponent>();
		if (ensure(LaserPointerComponent))
		{
			LaserPointerComponent->SetLaserPointerActor(LaserPointerActor);
		}
		else
		{
			UE_LOG(LogHWGA, Warning, TEXT("Failed to find LaserPointerComponent on player pawn"))
		}
	}
}

void AHWGAPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
	
}
