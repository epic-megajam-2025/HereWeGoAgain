// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LaserPointerComponent.generated.h"


class AAttentionTrigger;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HEREWEGOAGAIN_API ULaserPointerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULaserPointerComponent();

	UFUNCTION(BlueprintCallable)
	void SetLaserPointerActive(bool bActive);

	UFUNCTION(BlueprintCallable)
	void SetLaserPointerActor(AAttentionTrigger* NewLaserPointerActor);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AAttentionTrigger* GetLaserPointerActor() const;
	
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StartPointingLaser(const FVector& PointingAt);
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_StopPointingLaser(const FVector& PointingAt);
	
	UFUNCTION(BlueprintNativeEvent)
	void PointLaserAt(const FVector& PointingFrom, const FVector& PointingAt, const FVector& SurfaceNormal);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LaserTraceDistance = 100000.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bLaserActive = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDebug_DrawTrace = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<ECollisionChannel> LaserTraceChannel = ECC_Visibility;
	
private:
	void PointLaser();
	TWeakObjectPtr<AAttentionTrigger> LaserPointerActor;
};
