// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_BaseProjctWaitEvent_OwnerBase.h"
#include"AvatarCharacter.h"
#include"DrawDebugHelpers.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include"BaseChar_AbilitySystemComponent.h"



UGA_BaseProjctWaitEvent_OwnerBase::UGA_BaseProjctWaitEvent_OwnerBase()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	AbilityInputID = EGASAbilityInputID::Primary_Attack;


	MuzzleLocationName = "Muzzle";
	MuzzleOffsetDistance = 25;

	//ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Player.Debuff.Stun")));
}
void UGA_BaseProjctWaitEvent_OwnerBase::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);
	UAnimMontage* MontageToPlay = AttackMontage;
	Hero = Cast<	AAvatarCharacter>(GetAvatarActorFromActorInfo());
	ProjectileClass = Hero->ProjectileClass;
	DamageGameplayEffect = Hero->DamageGameplayEffect;
	if (Hero)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "RepFire");
		FString MeleeStartSection = "Throw";
		//FName MontageSectionName = FName(*MeleeStartSection);
		Task = UAT_PlayMontageAndWaitForEvent::PlayMontageAndWaitForEvent(this, NAME_None, MontageToPlay, FGameplayTagContainer(), 1.0f, FName(*MeleeStartSection), false, 1.0f);
		Task->OnBlendOut.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::OnCompletedOrOnCancelled);
		Task->OnCompleted.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::OnCompletedOrOnCancelled);
		Task->OnInterrupted.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::OnCompletedOrOnCancelled);
		Task->OnCancelled.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::OnCompletedOrOnCancelled);
		Task->EventReceived.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::EventReceived);
		// ReadyForActivation() is how you activate the AbilityTask in C++. Blueprint has magic from K2Node_LatentGameplayTaskCall that will automatically call ReadyForActivation().
		Task->ReadyForActivation();
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}

void UGA_BaseProjctWaitEvent_OwnerBase::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{


	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

AAvatarCharacter* UGA_BaseProjctWaitEvent_OwnerBase::FindClosestTargetToHero()
{
	TArray<AActor*> _TempArryActors;
	float ClosestDis = 1000000000;
	AAvatarCharacter* Target = nullptr;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAvatarCharacter::StaticClass(), _TempArryActors);
	for (int32 i = 0; i < _TempArryActors.Num(); i++)
	{
		if (_TempArryActors[i] != Hero)
		{
			AAvatarCharacter* Villan = Cast<AAvatarCharacter>(_TempArryActors[i]);
			float dist = FVector::Dist(Hero->GetActorLocation(), _TempArryActors[i]->GetActorLocation());
			if (dist < ClosestDis)
			{

				ClosestDis = dist;
				Target = Villan;

			}
		}
	}
	if (Target)
	{
		return Target;
	}
	return nullptr;
}

void UGA_BaseProjctWaitEvent_OwnerBase::OnCompletedOrOnCancelled(FGameplayTag EventTag, FGameplayEventData EventData)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_BaseProjctWaitEvent_OwnerBase::EventReceived(FGameplayTag EventTag, FGameplayEventData EventData)
{
	if (ProjectileClass)
	{
		// Get the camera transform.

		FVector CameraLocation;
		FRotator CameraRotation;
		Hero->GetActorEyesViewPoint(CameraLocation, CameraRotation);
		CameraLocation = Hero->GetMesh()->GetSocketLocation(FName(*MuzzleLocationName));

		FHitResult Hit;

		// We set up a line trace from our current location to a point 1000cm ahead of us
		FVector TraceStart = CameraLocation;
		//FVector CameFor =Hero->GetCamForwardVector()
		FVector TraceEnd = TraceStart + Hero->GetCamForwardVector() * 100000.0f;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Hero);


		// Set MuzzleOffset to spawn projectiles slightly in front of the camera.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		if (!bIsCameraControlled)
		{
			CameraRotation = Hero->GetActorRotation();
		}

		// Transform MuzzleOffset from camera space to world space.
		FVector MuzzleLocation = CameraLocation + (Hero->GetActorForwardVector() * MuzzleOffsetDistance);

		// Skew the aim to be slightly upwards.
		FRotator MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, TraceEnd);

		if (GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Hit Line");
			FVector HitLoc = Hit.ImpactPoint;
			MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, HitLoc);
		}
		//MuzzleRotation.Pitch += 10.0f;
		//Hero->IncrementComboCount();



		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = Hero;
			SpawnParams.Instigator = Hero->GetInstigator();

			if (GetCurrentActivationInfo().ActivationMode == EGameplayAbilityActivationMode::Authority)
			{
				if (!Projectile)
				{
					// Spawn the projectile at the muzzle.
				  // GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "Event recied");
					Projectile = World->SpawnActor<AGAS_HitActor>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
					Projectile->SetOwner(GetAvatarActorFromActorInfo());
					FGameplayCueParameters ImpactParams;
					ImpactParams.Location = Hero->GetActorLocation();


					FGameplayTag TagImpact = FGameplayTag::RequestGameplayTag(FName(GCTagForMuzzle));
					if (TagImpact.IsValid())
					{
						Hero->GetAbilitySystemComponent()->AddGameplayCue(TagImpact, ImpactParams);
					}
					if (Projectile)
					{
						GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, "Fire");

						// Set the projectile's initial trajectory.



						FVector LaunchDirection = MuzzleRotation.Vector();
						Projectile->FireInDirection(LaunchDirection);

						if (bIsAHomingProjectile == true)
						{
							AAvatarCharacter* HomingTar = FindClosestTargetToHero();
							if (HomingTar)
							{
								Projectile->HomingTargetSet(HomingTar, HomingAccuracy);
							}
						}

						Projectile->OnDestroyed.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::ProjectileDestroyed);
						FGameplayTag TagX = FGameplayTag::RequestGameplayTag(FName("Weapon.Projectile.Hit"));

						TaskHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TagX);
						TaskHit->EventReceived.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::HitEventReceived);
						TaskHit->ReadyForActivation();



					}
					else
					{
						EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

					}
				}
			}



		}
		else
		{
			EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

		}
	}
	else
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);

	}
	FGameplayTag TagX = FGameplayTag::RequestGameplayTag(FName("Weapon.Projectile.Hit"));
	TaskHit = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, TagX);
	TaskHit->EventReceived.AddDynamic(this, &UGA_BaseProjctWaitEvent_OwnerBase::HitEventReceived);
	TaskHit->ReadyForActivation();

}

void UGA_BaseProjctWaitEvent_OwnerBase::HitEventReceived(FGameplayEventData EventData)
{
	FGameplayAbilityTargetDataHandle Handle;
	Handle.Append(EventData.TargetData);


	const AAvatarCharacter* Villan = Cast<	AAvatarCharacter>(EventData.Target);

	if (Villan && GetCurrentActivationInfo().ActivationMode == EGameplayAbilityActivationMode::Authority)
	{
		/*FGameplayCueParameters ImpactParams;
		ImpactParams.Location = Villan->GetActorLocation();

		FGameplayTag TagImpact = FGameplayTag::RequestGameplayTag(FName(GCTagForImpact));

		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Effect");
		Villan->GetAbilitySystemComponent()->AddGameplayCue(TagImpact, ImpactParams);*/

		ApplyGameplayEffectToTarget(GetCurrentAbilitySpecHandle(), CurrentActorInfo, CurrentActivationInfo, EventData.TargetData, DamageGameplayEffect, 1);


	}

}

void UGA_BaseProjctWaitEvent_OwnerBase::ProjectileDestroyed(AActor* DestroyedActor)
{
	Projectile = nullptr;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UGA_BaseProjctWaitEvent_OwnerBase::OnFinishWait()
{
	Projectile = nullptr;
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}



