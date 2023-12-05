// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_GatewayAbility.h"
#include"AvatarCharacter.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"

UGA_GatewayAbility::UGA_GatewayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityInputID = EGASAbilityInputID::Ability_1;

	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Attack"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
	MaxTimeInTeleport = 5.0f;
}
void UGA_GatewayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	Hero = Cast<	AAvatarCharacter>(GetAvatarActorFromActorInfo());
	
		TeleportLocation = Hero->GetActorLocation();
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = Hero;
		SpawnParams.Instigator = Hero->GetInstigator();
		if (TeleporterActorClass)
		{
			TeleporterActor = GetWorld()->SpawnActor<AActor>(TeleporterActorClass, Hero->GetActorLocation(), Hero->GetActorRotation(), SpawnParams);
		}
	
	
	
	UAbilityTask_WaitInputPress* inputPressTask = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
	inputPressTask->OnPress.AddDynamic(this, &UGA_GatewayAbility::OnInputRelEvent);
	inputPressTask->ReadyForActivation();

	UAbilityTask_WaitDelay* WaitTaskDelay= UAbilityTask_WaitDelay::WaitDelay(this, MaxTimeInTeleport);
	WaitTaskDelay->OnFinish.AddDynamic(this, &UGA_GatewayAbility::WaitMaxOver);
	WaitTaskDelay->ReadyForActivation();
	
}

void UGA_GatewayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_GatewayAbility::WaitMaxOver()
{
	TeleportBack();
}

void UGA_GatewayAbility::OnInputRelEvent(float TimeHeld)
{
	TeleportBack();
}

void UGA_GatewayAbility::TeleportBack()
{
	if (TeleporterActor)
	{
		TeleporterActor->Destroy();
		Hero->SetActorLocation(TeleportLocation);
		
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	else
	{
		Hero->SetActorLocation(TeleportLocation);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}