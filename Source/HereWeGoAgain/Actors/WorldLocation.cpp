// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldLocation.h"

#include "Subsystems/WorldLocationSubsystem.h"


AWorldLocation::AWorldLocation()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AWorldLocation::BeginPlay()
{
	Super::BeginPlay();
	if (LocationTag.IsValid())
		if (auto WLS = GetWorld()->GetSubsystem<UWorldLocationSubsystem>())
			WLS->RegisterWorldLocation(LocationTag, this);
}

void AWorldLocation::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (LocationTag.IsValid())
		if (auto World = GetWorld())
			if (auto WLS = World->GetSubsystem<UWorldLocationSubsystem>())
				WLS->UnregisterWorldLocation(LocationTag, this);
	
	Super::EndPlay(EndPlayReason);
}
