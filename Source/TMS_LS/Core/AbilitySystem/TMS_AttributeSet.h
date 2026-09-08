// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "TMS_AttributeSet.generated.h"

#define TMS_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class TMS_LS_API UTMS_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	UTMS_AttributeSet();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_Health", Category = "Attributes")
	FGameplayAttributeData Health;
	TMS_ATTRIBUTE_ACCESSORS(UTMS_AttributeSet, Health);
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_MaxHealth", Category = "Attributes")
	FGameplayAttributeData MaxHealth;
	TMS_ATTRIBUTE_ACCESSORS(UTMS_AttributeSet, MaxHealth);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_Stamina", Category = "Attributes")
	FGameplayAttributeData Stamina;
	TMS_ATTRIBUTE_ACCESSORS(UTMS_AttributeSet, Stamina);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_MaxStamina", Category = "Attributes")
	FGameplayAttributeData MaxStamina;
	TMS_ATTRIBUTE_ACCESSORS(UTMS_AttributeSet, MaxStamina);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_Armor", Category = "Attributes")
	FGameplayAttributeData Armor;
	TMS_ATTRIBUTE_ACCESSORS(UTMS_AttributeSet, Armor);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing = "OnRep_MaxArmor", Category = "Attributes")
	FGameplayAttributeData MaxArmor;
	TMS_ATTRIBUTE_ACCESSORS(UTMS_AttributeSet, MaxArmor);

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& Previous)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UTMS_AttributeSet, Health, Previous);
	}
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& Previous)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UTMS_AttributeSet, Health, Previous);
	}
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& Previous)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UTMS_AttributeSet, Health, Previous);
	}
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& Previous)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UTMS_AttributeSet, Health, Previous);
	}
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& Previous)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UTMS_AttributeSet, Health, Previous);
	}
	UFUNCTION()
	void OnRep_MaxArmor(const FGameplayAttributeData& Previous)
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(UTMS_AttributeSet, Health, Previous);
	}
};
