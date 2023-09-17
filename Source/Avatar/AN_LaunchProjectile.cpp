// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_LaunchProjectile.h"
#include"AvatarCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"

void UAN_LaunchProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	AAvatarCharacter* MyActor = Cast<AAvatarCharacter>(MeshComp->GetOwner());
	FGameplayEventData Pay;
	Pay.Instigator = MyActor;
	Pay.Target = MyActor;
	FGameplayAbilityTargetDataHandle TDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(MyActor);
	//FGameplayAbilityTargetData TData;
	Pay.TargetData = TDataHandle;

	FGameplayTag TagX = FGameplayTag::RequestGameplayTag(FName("Weapon.Projectile.Launch"));
	//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TagX.ToString());
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(MyActor, TagX, Pay);
}