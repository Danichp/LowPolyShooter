// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_WeaponDistanceService.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"

UTMS_WeaponDistanceService::UTMS_WeaponDistanceService()
{
	NodeName = "Weapon Distance Service";
}

void UTMS_WeaponDistanceService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const auto Controller = OwnerComp.GetAIOwner();
	const auto Blackboard = OwnerComp.GetBlackboardComponent();

	
	if (Controller && Controller->GetPawn())
	{
		float MinDistance = 0.f;
		float MaxDistance = 100.f;
		const UTMS_WeaponComponent* WC = Controller->GetPawn()->GetComponentByClass<UTMS_WeaponComponent>();
		if (WC && WC->CurrentWeapon)
		{
			if (ATMS_FireWeapon* FW = Cast<ATMS_FireWeapon>(WC->CurrentWeapon))
			{
				float Diff = FMath::Abs(FW->Distance - FW->EffectiveDistance);
				MinDistance = FW->EffectiveDistance - Diff/2;
				MaxDistance = FW->EffectiveDistance + Diff/2;
			}
		}
		
		Blackboard->SetValueAsFloat(MinDistanceKey.SelectedKeyName, MinDistance);
		Blackboard->SetValueAsFloat(MaxDistanceKey.SelectedKeyName, MaxDistance);
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
