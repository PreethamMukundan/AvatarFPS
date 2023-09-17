// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseChar_MeleeAttack.h"
#include "AbilitySystemComponent.h"
#include "Avatar.h"
#include"DrawDebugHelpers.h"
#include "AvatarCharacter.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "AbilitySystemBlueprintLibrary.h"

UGA_BaseChar_MeleeAttack::UGA_BaseChar_MeleeAttack()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityInputID = EGASAbilityInputID::Primary_Attack;

	/*FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Attack"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.State.Dead")));*/
}
void UGA_BaseChar_MeleeAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo * ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData * TriggerEventData)
{
	UAnimMontage* MontageToPlay = AttackMontage;

	//ApplyCost(Handle, ActorInfo, ActivationInfo);
	//ApplyCooldown(Handle, ActorInfo, ActivationInfo);

	// Play fire montage and wait for event telling us to spawn the projectile


	Hero = Cast<	AAvatarCharacter>(GetAvatarActorFromActorInfo());
	//Hero->AttackCollisionComp->OnComponentEndOverlap.AddDynamic(this, &UGA_BaseChar_MeleeAttack::AttackOnOverlapEnd);


	if (Hero)
	{
		int32 StartSectionCount = Hero->ComboCount;
		FString MeleeStartSection = "Melee_" + FString::FromInt(StartSectionCount);
		FName MontageSectionName = FName(*MeleeStartSection);

		UAT_PlayMontageAndWaitForEvent* Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, MontageSectionName, false, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &UGA_BaseChar_MeleeAttack::OnCompleted);
		Task->OnCompleted.AddDynamic(this, &UGA_BaseChar_MeleeAttack::OnCompleted);
		Task->OnInterrupted.AddDynamic(this, &UGA_BaseChar_MeleeAttack::OnCancelled);
		Task->OnCancelled.AddDynamic(this, &UGA_BaseChar_MeleeAttack::OnCancelled);
		Task->EventReceived.AddDynamic(this, &UGA_BaseChar_MeleeAttack::EventReceived);
		// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
		Task->ReadyForActivation();
	}
}

void UGA_BaseChar_MeleeAttack::AttackOnOverlapBegin(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

	//ApplyCost(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo);
	//Hero->IncrementComboCount();
	//EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_BaseChar_MeleeAttack::AttackOnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	//const AZEROCharacter* Villan = Cast<	AZEROCharacter>(OtherActor);

	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Hit");
	////DrawDebugSphere(GetWorld(), Hero->GetActorLocation(), 32, 32, FColor::Blue, false, 20);


	//if (Hero->TeamID != Villan->TeamID)
	//{
	//	FGameplayAbilityTargetDataHandle TDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(OtherActor);
	//	ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, TDataHandle, DamageGameplayEffect, 1);
	//	//ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EventData.TargetData, DamageGameplayEffect, 1);
	//}
}

void UGA_BaseChar_MeleeAttack::OnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_BaseChar_MeleeAttack::OnCompleted(FGameplayTag EventTag, FGameplayEventData EventData)
{
	Hero->IncrementComboCount();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_BaseChar_MeleeAttack::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{

	//Hero->StopAnimMontage(AttackMontage);
	const AAvatarCharacter* Villan = Cast<	AAvatarCharacter>(EventData.Target);


	//DrawDebugSphere(GetWorld(), Hero->GetActorLocation(), 32, 32, FColor::Blue, false, 20);
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Append(EventData.TargetData);
	
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, "Hit");
		ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EventData.TargetData, DamageGameplayEffect, 1);
		Hero->IncrementComboCount();
	
	//ApplyCost(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}
