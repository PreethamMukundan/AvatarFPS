// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChar_BaseGameplayAbility.h"
#include "GA_AIRJordan.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API UGA_AIRJordan : public UBaseChar_BaseGameplayAbility
{
	GENERATED_BODY()
public:

	UGA_AIRJordan();

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	class AAvatarCharacter* Hero;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class AGameplayAbilityWorldReticle> ReticleActorClass;

	UFUNCTION()
		void OnTargetReceived(const FGameplayAbilityTargetDataHandle& Data);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		float JumpPower;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		float DirectionPower;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		float AOERadius;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

	UFUNCTION()
	void OnMovementRelEvent(EMovementMode NewMovementMode);

	UFUNCTION()
	void WaitAfterLandOver();
	
};
