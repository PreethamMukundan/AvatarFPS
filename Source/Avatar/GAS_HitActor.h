// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GAS_HitActor.generated.h"

UCLASS()
class AVATAR_API AGAS_HitActor : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AGAS_HitActor();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		class USphereComponent* CollisionComp;

	

	TSubclassOf<class UGameplayEffect> DamageGameplayEffect;

	UPROPERTY(VisibleAnywhere, Category = Movement)
		class UProjectileMovementComponent* ProjectileMovementComponent;


	UPROPERTY(VisibleAnywhere, Category = Mesh)
		class UStaticMeshComponent* MeshComp;

	void FireInDirection(const FVector& ShootDirection);

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);


	void HomingTargetSet(class AAvatarCharacter* Target, float HomingAccuracy);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FString GCTagForImpact;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
