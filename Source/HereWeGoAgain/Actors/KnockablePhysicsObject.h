// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "KnockablePhysicsObject.generated.h"

class UBoxComponent;

UCLASS()
class HEREWEGOAGAIN_API AKnockablePhysicsObject : public AActor
{
	GENERATED_BODY()

public:
	AKnockablePhysicsObject();

protected:
	virtual void PostInitializeComponents() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAudioComponent* AudioComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* NavAreaBoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FallZHitThreshold = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FallAngleThreshold = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float FallSpeedThreshold = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ObjectHitAISoundTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AINoiseSoundRange = 650.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AINoiseLoudness = 1.f;
	
private:
	bool IsFallen(float Z) const;

	UFUNCTION()
	void OnObjectHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	float LastHitZ = FLT_MAX;
};
