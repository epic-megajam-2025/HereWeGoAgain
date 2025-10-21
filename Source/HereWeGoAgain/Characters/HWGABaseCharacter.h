// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Character.h"
#include "Interfaces/GestureCharacter.h"
#include "HWGABaseCharacter.generated.h"

class UBaseGesturesComponent;
class UGameplayAbility;
class UGameplayEffect;
class UMovementAttributeSet;

UCLASS()
class HEREWEGOAGAIN_API AHWGABaseCharacter : public ACharacter, public IAbilitySystemInterface, public IGestureCharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AHWGABaseCharacter(const FObjectInitializer& ObjectInitializer);
	UMovementAttributeSet* GetMovementAttributeSet() const;

	UFUNCTION(BlueprintCallable)
	void AddGameplayTag(const FGameplayTag& Tag, bool bUnique);

	UFUNCTION(BlueprintCallable)
	void RemoveGameplayTag(const FGameplayTag& Tag);

	virtual void FaceRotation(FRotator NewControlRotation, float DeltaTime = 0) override;

	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;
	virtual void Falling() override;
	virtual void Landed(const FHitResult& Hit) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UHWGAAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBaseGesturesComponent* GesturesComponent;
	
	UPROPERTY()
	UMovementAttributeSet* MovementAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> InitializationGameplayEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSoftClassPtr<UGameplayAbility>> GrantedAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UGesturesDataAsset> GesturesDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer CharacterTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagQuery StrafeWhenCharacterInState;

	// this value is multiplied by CMC->RotationRate.Yaw, so it should be small-ish
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RotateToInterpolationRate = 0.03f;
	
	UFUNCTION(BlueprintNativeEvent)
	void OnGameplayTagsChanged();
	
	UFUNCTION(BlueprintNativeEvent)
	void SetStrafing(bool bRequireStrafing);
	
protected:
	static FName AbilitySystemComponentName;
	static FName GesturesComponentName;
	
	void ChangeGameplayTags(const FGameplayTagContainer& DeltaTags, bool bAppend);
	void ChangeGameplayTags(const FGameplayTag& ChangedTag, bool bAppend);
	void UpdateStrafing();
	
	bool Gesture(const FGameplayTag& GestureTag);
	void StopGesture();

public: // IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

public: // IGestureCharacter
	virtual void GestureStarted() override;
	virtual void GestureFinished(bool bSuccess) override;
	virtual UGesturesDataAsset* GetGestures() const override;
	virtual void SetMovementEnabledForGestures(bool bEnabled) override;
	virtual void GetGestureCharacterActiveTags(FGameplayTagContainer& OutTags) const override;
	virtual void SetRightHandGesturePositioning(const FVector& RightHandWorldLocation) override;
	virtual void SetLeftHandGesturePositioning(const FVector& LeftHandWorldLocation) override;
	virtual void ResetRightHandGesturePositioning() override;
	virtual void ResetLeftHandGesturePositioning() override;
	virtual bool IsGameLoading_Gesture() const override;

public: // IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
};