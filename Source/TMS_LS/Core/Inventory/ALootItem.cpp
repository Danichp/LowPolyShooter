// Fill out your copyright notice in the Description page of Project Settings.


#include "ALootItem.h"

#include "TMS_InventoryComponent.h"
#include "TMS_InventorySubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"


// Sets default values
AALootItem::AALootItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	Root->SetupAttachment(GetRootComponent());

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AALootItem::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(ItemClass))
	{
		InitLoot();
	}
}

void AALootItem::InitLoot()
{
	if (!IsValid(ItemClass)) return;

	HoldingItem = NewObject<UItemObject>(this, ItemClass);

	if (!HoldingItem) return;
		
	if (HoldingItem->ItemData.PreviewMesh.IsPending())
	{
		FStreamableManager& SM = UAssetManager::GetStreamableManager();
		SM.RequestAsyncLoad(HoldingItem->ItemData.PreviewMesh.ToSoftObjectPath(), FStreamableDelegate::CreateLambda(
			[this]()
			{
				this->Mesh->SetStaticMesh(HoldingItem->ItemData.PreviewMesh.Get());
			}));
	}
	else
	{
		Mesh->SetStaticMesh(HoldingItem->ItemData.PreviewMesh.Get());
	}
}

// Called every frame
void AALootItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AALootItem::TryInteract(TWeakObjectPtr<class APlayerController> InPC)
{
	if (!InPC.IsValid() || !IsValid(HoldingItem)) return false;
	bool bSuccess = false;
	
	UTMS_InventorySubsystem* IS = GetGameInstance()->GetSubsystem<UTMS_InventorySubsystem>();
	if (IS)
	{
		IS->AddItemToController(InPC.Get(), HoldingItem);
	}
	//InIC->AddItem(ItemSlotData,bSuccess);
	Destroy();
	return true;
}

