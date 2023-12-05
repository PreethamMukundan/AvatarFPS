// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChar_BaseGameplayAbility.h"
#include "GA_GatewayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API UGA_GatewayAbility : public UBaseChar_BaseGameplayAbility
{
	GENERATED_BODY()
public:

	UGA_GatewayAbility();

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	class AAvatarCharacter* Hero;

	FVector TeleportLocation;

	class AActor* TeleporterActor;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
		TSubclassOf<class AActor> TeleporterActorClass;

		UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		float MaxTimeInTeleport;

		UFUNCTION()
	void WaitMaxOver();

		UFUNCTION()
	void OnInputRelEvent(float TimeHeld);


		UFUNCTION()
		void TeleportBack();
};
