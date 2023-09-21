// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChar_BaseGameplayAbility.h"
#include "AT_PlayMontageAndWaitForEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include"GAS_HitActor.h"
#include "GA_BaseProjctWaitEvent_OwnerBase.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API UGA_BaseProjctWaitEvent_OwnerBase : public UBaseChar_BaseGameplayAbility
{
	GENERATED_BODY()
public:

	UGA_BaseProjctWaitEvent_OwnerBase();

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/** Native function, called if an ability ends normally or abnormally. If bReplicate is set to true, try to replicate the ending to the client/server */
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)override;

	class AAvatarCharacter* Hero;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MuzzleOffsetDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FString MuzzleLocationName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FString GCTagForMuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FString GCTagForImpact;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Homing)
		bool bIsCameraControlled;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Homing)
		bool bIsAHomingProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Homing)
		float HomingAccuracy;

	AAvatarCharacter* FindClosestTargetToHero();

	UAT_PlayMontageAndWaitForEvent* Task;

	UAbilityTask_WaitGameplayEvent* TaskHit;

	UAbilityTask_WaitDelay* TaskWaitDelay;

	
		TSubclassOf<UGameplayEffect> DamageGameplayEffect;

protected:

	UFUNCTION()
		void OnCompletedOrOnCancelled(FGameplayTag EventTag, FGameplayEventData EventData);

	UFUNCTION()
		void EventReceived(FGameplayTag EventTag, FGameplayEventData EventData);

	
		TSubclassOf<AGAS_HitActor> ProjectileClass;

	class AGAS_HitActor* Projectile;


	UFUNCTION()
		void HitEventReceived(FGameplayEventData EventData);



	UFUNCTION()
		void ProjectileDestroyed(AActor* DestroyedActor);

	UFUNCTION()
		void OnFinishWait();
	
};
