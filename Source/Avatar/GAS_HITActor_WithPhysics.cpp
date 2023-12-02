// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS_HITActor_WithPhysics.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include"DrawDebugHelpers.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include"AvatarCharacter.h"
#include"BaseChar_AbilitySystemComponent.h"

// Sets default values
AGAS_HITActor_WithPhysics::AGAS_HITActor_WithPhysics()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}


	

	// Use this component to drive this projectile's movement.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComp);
	ProjectileMovementComponent->InitialSpeed = 3000.0f;
	ProjectileMovementComponent->MaxSpeed = 3000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = true;
	ProjectileMovementComponent->Bounciness = 0.3f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetupAttachment(RootComponent);
	RootComponent = MeshComp;

	HomingCollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("HomingSphereComponent"));
	// Use a sphere as a simple collision representation.
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	// Set the sphere's collision radius.
	CollisionComp->InitSphereRadius(15.0f);
	// Set the root component to be the collision component.
	
	CollisionComp->SetupAttachment(RootComponent);



	//CollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAS_HITActor_WithPhysics::OnHit);

	HomingCollisionComp->SetupAttachment(RootComponent);
	HomingCollisionComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HomingCollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HomingCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AGAS_HITActor_WithPhysics::OnOverlapBegin);

	bReplicates = true;
	SetReplicates(true);
	SetReplicateMovement(true);
}

void AGAS_HITActor_WithPhysics::FireInDirection(const FVector& ShootDirection)
{
}

void AGAS_HITActor_WithPhysics::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AAvatarCharacter* Hero = Cast<AAvatarCharacter>(GetOwner());
	FGameplayCueParameters ImpactParams;
	ImpactParams.Location = Hit.Location;

	FGameplayTag TagImpact = FGameplayTag::RequestGameplayTag(FName(GCTagForImpact));

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Effect");
	Hero->GetAbilitySystemComponent()->AddGameplayCue(TagImpact, ImpactParams);
	if (OtherActor != this && OtherActor != GetOwner())
	{

		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "REP.Hit");

		AAvatarCharacter* Villan = Cast<AAvatarCharacter>(OtherActor);
		if (Villan)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "REP.Hit");
			FGameplayEventData Pay;
			Pay.Instigator = Hero;
			Pay.Target = Villan;
			FGameplayAbilityTargetDataHandle TDataHandle = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Villan);
			//FGameplayAbilityTargetData TData;
			Pay.TargetData = TDataHandle;

			FGameplayTag TagX = FGameplayTag::RequestGameplayTag(FName("Weapon.Projectile.Hit"));
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TagX.ToString());
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Hero, TagX, Pay);
			if (Villan)
			{
				FGameplayEffectContextHandle EffectContext = Hero->GetAbilitySystemComponent()->MakeEffectContext();
				EffectContext.AddSourceObject(this);
				EffectContext.AddInstigator(Hero, this);



				FGameplayEffectSpecHandle SpecHandle = Hero->GetAbilitySystemComponent()->MakeOutgoingSpec(DamageGameplayEffect, 1, EffectContext);

				//ro->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(SpecHandle, Villan->GetAbilitySystemComponent());
				Hero->GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), Villan->GetAbilitySystemComponent());
				//Hero->GetAbilitySystemComponent()->ApplyGameplayEffectToTarget(DamageGameplayEffect, Villan->GetAbilitySystemComponent(), 1, EffectContext);
			}

			Destroy();

		}
		else
		{
			Destroy();
			//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "HitSomthingElse");
		}
	}
	else
	{
		Destroy();
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "Hit Owner or self");
	}
}

void AGAS_HITActor_WithPhysics::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != GetOwner())
	{
		AAvatarCharacter* Villan = Cast<AAvatarCharacter>(OtherActor);
		if (Villan)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, "LockedOn");
			if (!HomingTargetAvatar)
			{
				HomingTargetAvatar = Villan;
				HomingTargetSet(HomingTargetAvatar, ProjectileHomingAccuracy);
			}
		}
	}
}

void AGAS_HITActor_WithPhysics::HomingTargetSet(AAvatarCharacter* Target, float HomingAccuracy)
{

	if (Target)
	{
		ProjectileMovementComponent->HomingTargetComponent = Target->GetRootComponent();
		ProjectileMovementComponent->bIsHomingProjectile = true;
		ProjectileMovementComponent->HomingAccelerationMagnitude = HomingAccuracy;
	}
}



// Called when the game starts or when spawned
void AGAS_HITActor_WithPhysics::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this, &AGAS_HITActor_WithPhysics::OnHit);
}

// Called every frame
void AGAS_HITActor_WithPhysics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

