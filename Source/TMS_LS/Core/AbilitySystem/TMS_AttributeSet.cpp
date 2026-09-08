// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"

UTMS_AttributeSet::UTMS_AttributeSet()
{
	Health = 100.f;
	MaxHealth = 100.f;
	Stamina = 100.f;
	MaxStamina = 100.f;
	Armor = 100.f;
	MaxArmor = 100.f;
}

void UTMS_AttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UTMS_AttributeSet, Health, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UTMS_AttributeSet, MaxHealth, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UTMS_AttributeSet, Stamina, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UTMS_AttributeSet, MaxStamina, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UTMS_AttributeSet, Armor, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME_CONDITION_NOTIFY(UTMS_AttributeSet, MaxArmor, COND_None, REPNOTIFY_OnChanged);
}

void UTMS_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		if (GetMaxHealth() > 0)
		{
			float CurrentHealth = GetHealth();
			float NewCurrentHealth = (CurrentHealth/GetMaxHealth()) * NewValue;
			SetHealth(FMath::Clamp(NewCurrentHealth, 0.f, NewValue));
		}
	}
	else if (Attribute == GetStaminaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxStamina());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		if (GetMaxStamina() > 0)
		{
			float CurrentStamina = GetStamina();
			float NewCurrentHealth = (CurrentStamina/GetMaxStamina()) * NewValue;
			SetStamina(FMath::Clamp(NewCurrentHealth, 0.f, NewValue));
		}
	}
	if (Attribute == GetArmorAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxArmor());
	}
	else if (Attribute == GetMaxArmorAttribute())
	{
		if (GetMaxArmor() > 0)
		{
			SetArmor(NewValue);
		}
	}
}

void UTMS_AttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(GetHealth());
	}
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		SetStamina(GetStamina());
	}
	else if (Data.EvaluatedData.Attribute == GetArmorAttribute())
	{
		SetArmor(GetArmor());
	}
}
