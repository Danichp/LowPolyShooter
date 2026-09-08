// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_EnemyCharacterBase.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "TMS_LS/Components/NPCConfigComponent.h"
#include "TMS_LS/Components/TMS_LootComponent.h"
#include "TMS_LS/Core/TMS_HUD.h"

ATMS_EnemyCharacterBase::ATMS_EnemyCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	LootComponent = CreateDefaultSubobject<UTMS_LootComponent>("LootComponent");
	
	NPCConfig = CreateDefaultSubobject<UNPCConfigComponent>("NPCConfig");
}

void ATMS_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void ATMS_EnemyCharacterBase::OnInventoryUpdated()
{
	if (InventoryComponent->IsEmpty())
	{
		SetLifeSpan(5.f);
		OnEnemyDestroyed.Broadcast();
	}
	else
	{
		SetLifeSpan(0.f);
	}
}

void ATMS_EnemyCharacterBase::OnDeath()
{
	Super::OnDeath();
	if (AAIController* AIC = Cast<AAIController>(GetController()))
	{
		AIC->BrainComponent->StopLogic("Death");
	}
	SetLifeSpan(0.f);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	GetMesh()->SetAllBodiesSimulatePhysics(true);

	InventoryComponent->OnInventoryUpdated.AddDynamic(this, &ATMS_EnemyCharacterBase::OnInventoryUpdated);
}

bool ATMS_EnemyCharacterBase::TryInteract(TWeakObjectPtr<class APlayerController> InPC)
{
	if (!HealthComponent || !HealthComponent->bDead) return false;
	if (ATMS_HUD* InHUD = Cast<ATMS_HUD>(InPC->GetHUD()))
	{
		InHUD->SetUIState(EUIState::EUIS_Loot);
		return true;
	}
	return false;
}
