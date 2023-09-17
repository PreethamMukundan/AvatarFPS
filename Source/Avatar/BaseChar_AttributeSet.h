// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BaseChar_AttributeSet.generated.h"



#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
 	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class AVATAR_API UBaseChar_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	UBaseChar_AttributeSet();

	// Attribute Set Overrides.
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	// Set Attributes to replicate.
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Used to create a local copy of Damage which is then subtracted from Current Health.
	UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
		FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, Damage)

		// Used to create a local copy of Healing which is then added to Current Health.
		UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", meta = (HideFromLevelInfos))
		FGameplayAttributeData Healing;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, Healing)

		// Holds the current value for Health.
		UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentHealth)
		FGameplayAttributeData CurrentHealth;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, CurrentHealth)

		// Holds the value for Maximum Health.
		UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_MaximumHealth)
		FGameplayAttributeData MaximumHealth;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, MaximumHealth)

		// Holds the value for Health Regeneration.
		UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_HealthRegeneration)
		FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, HealthRegeneration)

		UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentSpeed)
		FGameplayAttributeData CurrentSpeed;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, CurrentSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Health Attribute Set", ReplicatedUsing = OnRep_CurrentJump)
		FGameplayAttributeData CurrentJump;
	ATTRIBUTE_ACCESSORS(UBaseChar_AttributeSet, CurrentJump)

protected:

	void AdjustAttributeForMaxChange(FGameplayAttributeData& AffectedAttribute, const FGameplayAttributeData& MaxAttribute, float NewMaxValue, const FGameplayAttribute& AffectedAttributeProperty) const;

	UFUNCTION()
		virtual void OnRep_CurrentHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_CurrentSpeed(const FGameplayAttributeData& OldValue);


	UFUNCTION()
		virtual void OnRep_CurrentJump(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_MaximumHealth(const FGameplayAttributeData& OldValue);

	UFUNCTION()
		virtual void OnRep_HealthRegeneration(const FGameplayAttributeData& OldValue);


};
