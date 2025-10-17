// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HWGAPlayerController.generated.h"

class AAttentionTrigger;
class UInputMappingContext;
class UUserWidget;

/**
 *  Simple first person Player Controller
 *  Manages the input mapping context.
 *  Overrides the Player Camera Manager class.
 */
UCLASS(abstract)
class HEREWEGOAGAIN_API AHWGAPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** Constructor */
	AHWGAPlayerController();

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category="Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;
	
	virtual void OnPossess(APawn* InPawn) override;
	
	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAttentionTrigger* LaserPointerActor;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAttentionTrigger> LaserPointerWidgetClass;
};
