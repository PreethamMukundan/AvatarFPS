// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseChar_BaseGameplayAbility.h"
#include "GA_BaseChar_BuffSelf.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API UGA_BaseChar_BuffSelf : public UBaseChar_BaseGameplayAbility
{
	GENERATED_BODY()
	

public:
	UGA_BaseChar_BuffSelf();

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> BuffGameplayEffect;

	/** Actually activate ability, do not call this directly. We'll call it from APAHeroCharacter::ActivateAbilitiesWithTags(). */
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;


	class AAvatarCharacter* Hero;


};
