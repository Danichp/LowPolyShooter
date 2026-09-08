// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_LS/Core/TMS_InteractInterface.h"
#include "GameFramework/Actor.h"
#include "ItemDataTypes.h"
#include "TMS_Chest.generated.h"

class UItemObject;

UENUM(BlueprintType)
enum class EChestState : uint8
{
	ECS_None UMETA(DisplayName = "None"),
	ECS_Closed UMETA(DisplayName = "Closed Chest"),
	ECS_Opened UMETA(DisplayName = "Opened Chest"),
	ECS_Broken UMETA(DisplayName = "Broken Chest")
};

DECLARE_DELEGATE(FOnChestLootedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChestGeneratedLootSignature, int32, LootType);

UCLASS()
class TMS_LS_API ATMS_Chest : public AActor, public ITMS_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATMS_Chest();

	virtual bool TryInteract(TWeakObjectPtr<class APlayerController> InPC) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
private:
	UFUNCTION()
	void GenerateLoot();

	void InitVisuals();

	void SetChestState(EChestState InState);

	UFUNCTION()
	void OnRep_ChestState();
	
public:
	FOnChestLootedSignature OnChestLooted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UTMS_InventoryComponent> InventoryComponent;

	UPROPERTY(BlueprintAssignable)
	FOnChestGeneratedLootSignature OnChestGeneratedLoot;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> ChestMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	TSoftObjectPtr<UStaticMesh> Mesh;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	TSoftObjectPtr<UMaterialInterface> CMatRef;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Setup")
	float TargetHeight = 100.f;

	bool bMovingUp = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing="OnRep_ChestState")
	EChestState ChestState = EChestState::ECS_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "/Script/TMS_LS.ELootType"))
	int32 ChestLootType = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<TSubclassOf<UItemObject>> PossibleLoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (UIMin = 0, ClampMin = 0, UIMax = 255, ClampMax = 255))
	uint8 LootItemsAmount = 0;

private:
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> ChestMaterial;

	UPROPERTY(Replicated)
	bool bLooted = false;
};
