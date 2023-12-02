// Fill out your copyright notice in the Description page of Project Settings.


#include "GASTargetActor.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

AGASTargetActor::AGASTargetActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TraceRange = 1500.0f;
	bDestroyOnConfirmation = true;
	ShouldProduceTargetDataOnServer = true;
	MyReticleActor = nullptr;
}




void AGASTargetActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LineTraceFN(TraceHitResults);
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, "GASTARTick");
	if (MyReticleActor)
	{
		if (TraceHitResults.bBlockingHit)
		{
			MyReticleActor->SetActorLocation(TraceHitResults.ImpactPoint, false, nullptr, ETeleportType::None);
		}
		else
		{
			MyReticleActor->SetActorLocation(TraceHitResults.TraceEnd, false, nullptr, ETeleportType::None);
		}
	}
}

bool AGASTargetActor::LineTraceFN(FHitResult& TraceHitResult)
{
	FVector ViewPoint;
	FRotator ViewRotation;
	PrimaryPC->GetPlayerViewPoint(ViewPoint, ViewRotation);
	
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = true;
	APawn* MasterPawn = PrimaryPC->GetPawn();
	if (MasterPawn)
	{
		QueryParams.AddIgnoredActor(MasterPawn->GetUniqueID());
	}
	bool TryTrace = GetWorld()->LineTraceSingleByChannel(TraceHitResult, ViewPoint, ViewPoint + ViewRotation.Vector() * TraceRange, ECC_Visibility, QueryParams);
	return TryTrace;
}

void AGASTargetActor::StartTargeting(UGameplayAbility* Ability)
{
	OwningAbility = Ability;
	PrimaryPC = Cast<APlayerController>(Ability->GetOwningActorFromActorInfo()->GetInstigatorController());
	MyReticleActor = SpawnReticleActor(GetActorLocation(), GetActorRotation());
}

void AGASTargetActor::ConfirmTargetingAndContinue()
{
	FHitResult HitResult;
	bool TryTrace = LineTraceFN(HitResult);
	FGameplayAbilityTargetDataHandle TargetData;
	TargetData = StartLocation.MakeTargetDataHandleFromHitResult(OwningAbility, HitResult);
	if (TargetData != nullptr)
	{
		TargetDataReadyDelegate.Broadcast(TargetData);
	}
	else
	{
		TargetDataReadyDelegate.Broadcast(FGameplayAbilityTargetDataHandle());
	}
	DestroyReticleActors();
}

void AGASTargetActor::CancelTargeting()
{
	Super::CancelTargeting();
	DestroyReticleActors();
}

AGameplayAbilityWorldReticle* AGASTargetActor::SpawnReticleActor(FVector location, FRotator Rotation)
{
	if (ReticleClass)
	{
		if (!MyReticleActor)
		{
			AGameplayAbilityWorldReticle* SpawnedReticleActor = GetWorld()->SpawnActor<AGameplayAbilityWorldReticle>(ReticleClass, location, Rotation);

			if (SpawnedReticleActor)
			{
				SpawnedReticleActor->InitializeReticle(this, PrimaryPC, ReticleParams);

				return SpawnedReticleActor;
			}
		}
		else
		{
			return MyReticleActor;
		}
	}
	return nullptr;
}

void AGASTargetActor::DestroyReticleActors()
{
	if (MyReticleActor)
	{
		MyReticleActor->Destroy();
	}
}
