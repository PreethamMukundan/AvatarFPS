// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChar_BaseGameplayAbility.h"
#include "BaseChar_TempForDashAbilities.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API UBaseChar_TempForDashAbilities : public UBaseChar_BaseGameplayAbility
{
	GENERATED_BODY()
	
public:
	UBaseChar_TempForDashAbilities();


	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	class AAvatarCharacter* Hero;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		UAnimMontage* DashMontage;

	UPROPERTY(EditAnywhere)
		UCurveFloat* DashFloatCurve;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> InvisGameplayEffect;
protected:


	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		float DistanceOfTeleport;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		float TimeOfTeleport;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		bool bDoesKeyPressMatter;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		bool bCanUseInAir;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "Distance")
		bool bCanPassThroughPlayers;

	UFUNCTION()
		void LocationReached();

};
