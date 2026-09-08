// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_Chest.h"

#include "TMS_InventoryComponent.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ItemObjects/ItemObject.h"
#include "Net/UnrealNetwork.h"
#include "TMS_LS/Core/TMS_HUD.h"


// Sets default values
ATMS_Chest::ATMS_Chest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetNetUpdateFrequency(0.5f);
	SetMinNetUpdateFrequency(0.5f);

	ChestMesh = CreateDefaultSubobject<UStaticMeshComponent>("ChestMesh");
	ChestMesh->SetupAttachment(RootComponent);

	InventoryComponent = CreateDefaultSubobject<UTMS_InventoryComponent>("InventoryComponent");
}

bool ATMS_Chest::TryInteract(TWeakObjectPtr<class APlayerController> InPC)
{
	if (ATMS_HUD* InHUD = Cast<ATMS_HUD>(InPC->GetHUD()))
	{
		InHUD->SetUIState(EUIState::EUIS_Loot);
	}
	return false;
}

// Called when the game starts or when spawned
void ATMS_Chest::BeginPlay()
{
	Super::BeginPlay();

	InitVisuals();
	
	GenerateLoot();
}

void ATMS_Chest::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ATMS_Chest, ChestState, COND_None, REPNOTIFY_OnChanged);
	DOREPLIFETIME(ATMS_Chest, bLooted);
}

#if WITH_EDITOR
void ATMS_Chest::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	InitVisuals();
}
#endif

// Called every frame
void ATMS_Chest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATMS_Chest::GenerateLoot()
{
	if (bLooted)
		return;

	if (PossibleLoot.IsEmpty()) return;
	
	for (int32 i = 0; i < LootItemsAmount; ++i)
	{
		int32 RandID = FMath::RandRange(0, PossibleLoot.Num()-1);
		if (TSubclassOf<UItemObject> RandItem = PossibleLoot[RandID])
		{
			bool bSuccess = false;

			InventoryComponent->AddItem(PossibleLoot[RandID], 1, bSuccess);
		}
	}
	
	bLooted = true;
	OnChestLooted.ExecuteIfBound();
	OnChestGeneratedLoot.Broadcast(ChestLootType);
}

void ATMS_Chest::InitVisuals()
{
	FStreamableManager& SM = UAssetManager::GetStreamableManager();

	if (Mesh.IsPending())
	{
		SM.RequestAsyncLoad(Mesh.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
			[this]()
			{
				ChestMesh->SetStaticMesh(Mesh.Get());
			}));
	}
	else
	{
		ChestMesh->SetStaticMesh(Mesh.Get());
	}
	
	if (CMatRef.IsPending())
	{
		SM.RequestAsyncLoad(CMatRef.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
			[this]()
			{
				ChestMaterial = UMaterialInstanceDynamic::Create(CMatRef.Get(), this);
				if (IsValid(ChestMaterial))
				{
					ChestMesh->SetMaterial(0, ChestMaterial);
				}
			}));
	}
	else
	{
		ChestMaterial = UMaterialInstanceDynamic::Create(CMatRef.Get(), this);
		if (IsValid(ChestMaterial))
		{
			ChestMesh->SetMaterial(0, ChestMaterial);
		}
	}
}

void ATMS_Chest::SetChestState(EChestState InState)
{
	ChestState = InState;
	switch (ChestState)
	{
	case EChestState::ECS_Closed:
		ChestMaterial->SetVectorParameterValue("Color", FColor::Red);
		break;
	case EChestState::ECS_Opened:
		ChestMaterial->SetVectorParameterValue("Color", FColor::Green);
		break;
	case EChestState::ECS_Broken:
		ChestMaterial->SetVectorParameterValue("Color", FColor::Cyan);
		break;
	default:
		break;
	}
}

void ATMS_Chest::OnRep_ChestState()
{
	SetChestState(ChestState);
}

