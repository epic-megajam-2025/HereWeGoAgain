// Fill out your copyright notice in the Description page of Project Settings.


#include "CatController.h"


// Sets default values
ACatController::ACatController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACatController::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACatController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

