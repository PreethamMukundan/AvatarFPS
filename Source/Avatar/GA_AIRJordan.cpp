// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_AIRJordan.h"
#include "Abilities/Tasks/AbilityTask_WaitMovementModeChange.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include"GASTargetActor.h"
#include"AvatarCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"

UGA_AIRJordan::UGA_AIRJordan()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	//AbilityInputID = EGASAbilityInputID::Ability_2;

	FGameplayTag Ability1Tag = FGameplayTag::RequestGameplayTag(FName("Ability.Skill.Attack"));
	AbilityTags.AddTag(Ability1Tag);
	ActivationOwnedTags.AddTag(Ability1Tag);

	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.Skill")));
	JumpPower = 1500.0f;
	DirectionPower = 1000.0f;
}

void UGA_AIRJordan::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	
	Hero = Cast<	AAvatarCharacter>(GetAvatarActorFromActorInfo());
	UAbilityTask_WaitMovementModeChange* MovementModeTask = UAbilityTask_WaitMovementModeChange::CreateWaitMovementModeChange(this, EMovementMode::MOVE_Walking);
	MovementModeTask->OnChange.AddDynamic(this, &UGA_AIRJordan::OnMovementRelEvent);
	MovementModeTask->ReadyForActivation();

	if (Hero->GetCharacterMovement()->IsMovingOnGround())
	{
		FVector StartLoc = Hero->GetActorLocation();
		FVector EndLoc = FVector(0, 0, JumpPower);
		
			//Getting the Direction the player is moving
			if (Hero->GetCharacterMovement()->GetCurrentAcceleration() != FVector(0, 0, 0))
			{
				FVector temp= Hero->GetCharacterMovement()->GetCurrentAcceleration().GetSafeNormal() * DirectionPower;
				EndLoc = FVector(temp.X, temp.Y, JumpPower);
			}
			else
			{
				//if the player is not moving make end point behind him
				EndLoc = FVector(0, 0, JumpPower);
			}
			Hero->LaunchCharacter(EndLoc, true, true);
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
	/*UAbilityTask_WaitInputRelease* InputTask = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	InputTask->OnRelease.AddDynamic(this, &UGA_AIRJordan::OnInputRelEvent);
	InputTask->ReadyForActivation();*/
	//Important code
	/*FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Hero;
	SpawnParams.Instigator = Hero->GetInstigator();
	AGASTargetActor* TargetAct = GetWorld()->SpawnActor<AGASTargetActor>(AGASTargetActor::StaticClass(), FVector(0, 0, 0), FRotator(0, 0, 0), SpawnParams);
	TargetAct->ReticleClass = ReticleActorClass;
	UAbilityTask_WaitTargetData* TaskTarget = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(this, NAME_None, EGameplayTargetingConfirmation::UserConfirmed,TargetAct);
	TaskTarget->ValidData.AddDynamic(this, &UGA_AIRJordan::OnTargetReceived);
	TaskTarget->ReadyForActivation();
	TargetAct->PrimaryActorTick.bCanEverTick = true;*/


}

void UGA_AIRJordan::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_AIRJordan::OnTargetReceived(const FGameplayAbilityTargetDataHandle& Data)
{
	
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_AIRJordan::OnMovementRelEvent(EMovementMode NewMovementMode)
{
	 TArray < TEnumAsByte < EObjectTypeQuery > > BangObjectTypes;
	//BangObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	BangObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray < AActor* > BangActorsToIgnore;
	BangActorsToIgnore.Add(Hero);

	TArray < AActor* > BangOutActors;

	//getting all the player around the user 
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Hero->GetActorLocation(),AOERadius, BangObjectTypes, AAvatarCharacter::StaticClass(), BangActorsToIgnore, BangOutActors);
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Black, "Activate");
	//Hero->LandingCollisionComp->GetOverlappingActors(BangOutActors, AAvatarCharacter::StaticClass());
	for (AActor* Target : BangOutActors)
	{
		
		//checking if the actor is a ZEROCharacter to have a extra layer of check even though the overlap function only returns ZEROCharacters
		AAvatarCharacter* ZTarget = Cast<AAvatarCharacter>(Target);

		if (ZTarget )
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Loop");
				//Applying gameplay effect
				FGameplayAbilityTargetDataHandle TDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(ZTarget);
				ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, TDataHandle, DamageGameplayEffect, 1);
			
		}

	}
	DrawDebugSphere(GetWorld(), Hero->GetActorLocation(), AOERadius, 32, FColor::Cyan, false, 2);
	UAbilityTask_WaitDelay* WaitTask = UAbilityTask_WaitDelay::WaitDelay(this, 2.0f);
	WaitTask->OnFinish.AddDynamic(this, &UGA_AIRJordan::WaitAfterLandOver);
	WaitTask->ReadyForActivation();
}

void UGA_AIRJordan::WaitAfterLandOver()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}


