// Fill out your copyright notice in the Description page of Project Settings.


#include "NatureCharacter.h"
#include"BaseChar_BaseGameplayAbility.h"
#include "AbilitySystemBlueprintLibrary.h"


void ANatureCharacter::AttackAbilityBoostTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	if (NewCount > 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "NatureBuff");
		ProjectileClass = BuffedProjectileClass;
		DamageGameplayEffect = BuffedDamageGameplayEffect;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "NatureDeBuff");
		ProjectileClass = NormalProjectileClass;
		DamageGameplayEffect = NormalDamageGameplayEffect;
	}
}
