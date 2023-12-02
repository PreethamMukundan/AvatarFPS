// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "GASTargetActor.generated.h"

/**
 * 
 */
UCLASS()
class AVATAR_API AGASTargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()

public:

	AGASTargetActor();

	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	 AGameplayAbilityWorldReticle* MyReticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true), Category = "GAS")
		float TraceRange;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GAS")
		FHitResult TraceHitResults;

	bool LineTraceFN(FHitResult& TraceHitResult);

	virtual void StartTargeting(UGameplayAbility* Ability) override;
	virtual void ConfirmTargetingAndContinue() override;
	virtual void CancelTargeting() override;

	virtual AGameplayAbilityWorldReticle* SpawnReticleActor(FVector location, FRotator Rotation);
	virtual void DestroyReticleActors();
	
};
