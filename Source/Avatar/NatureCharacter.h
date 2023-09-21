// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvatarCharacter.h"
#include "NatureCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API ANatureCharacter : public AAvatarCharacter
{
	GENERATED_BODY()

public:
	

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AGAS_HitActor> BuffedProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> BuffedDamageGameplayEffect;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AGAS_HitActor> NormalProjectileClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
		TSubclassOf<UGameplayEffect> NormalDamageGameplayEffect;

	
		virtual void AttackAbilityBoostTagChanged(const FGameplayTag CallbackTag, int32 NewCount) override;

	
};
