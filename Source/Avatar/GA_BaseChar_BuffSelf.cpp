// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseChar_BuffSelf.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include"BaseChar_AbilitySystemComponent.h"
#include "AvatarCharacter.h"

UGA_BaseChar_BuffSelf::UGA_BaseChar_BuffSelf()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityInputID = EGASAbilityInputID::Primary_Attack;

}

void UGA_BaseChar_BuffSelf::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Hero = Cast<	AAvatarCharacter>(GetAvatarActorFromActorInfo());
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, "XBuff");
	if (BuffGameplayEffect)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, "Buff");
		FGameplayAbilityTargetDataHandle TDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Hero);
		ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, TDataHandle, BuffGameplayEffect, 1);
		
	}

	//ApplyCost(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}