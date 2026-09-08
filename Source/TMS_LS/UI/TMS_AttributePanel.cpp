// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_AttributePanel.h"

#include "TMS_LS/Components/TMS_HealthComponent.h"

void UTMS_AttributePanel::NativeConstruct()
{
	Super::NativeConstruct();

	check(HealthBar);
	check(StaminaBar);

	if (!GetOwningPlayerPawn()) return;
	HealthComponent = GetOwningPlayerPawn()->GetComponentByClass<UTMS_HealthComponent>();
	if (!HealthComponent) return;
	FTimerHandle UpdateHandle;
	GetWorld()->GetTimerManager().SetTimer(UpdateHandle,this,
		&UTMS_AttributePanel::UpdateData,
		1.f/HealthComponent->UpdateFrequency, true);
}

void UTMS_AttributePanel::UpdateData()
{
	if (!HealthComponent) return;
	HealthBar->SetPercent(HealthComponent->GetHealthPercentage());
	StaminaBar->SetPercent(HealthComponent->GetStaminaPercentage());
}
