// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Avatar.h"
#include "BaseChar_BaseGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API UBaseChar_BaseGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:
	UBaseChar_BaseGameplayAbility();

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Ability")
		EGASAbilityInputID AbilityInputID = EGASAbilityInputID::None;
};
