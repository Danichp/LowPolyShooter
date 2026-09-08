// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataTypes.h"
#include "Components/ActorComponent.h"
#include "TMS_InventoryComponent.generated.h"

class UItemObject;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryUpdatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryInitializedSignature);

UCLASS( BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TMS_LS_API UTMS_InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTMS_InventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(BlueprintAssignable)
    FOnInventoryUpdatedSignature OnInventoryUpdated;
    UPROPERTY(BlueprintAssignable)
    FOnInventoryInitializedSignature OnInventoryInitialized;
    
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ContainerName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<TObjectPtr<UItemObject>> Slots;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 ContainerSize = 10;

	/*UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 StartDrag;*/
	
public:
	UFUNCTION(BlueprintCallable)
	void AddItem(TSubclassOf<UItemObject> InItemClass, int32 Amount , bool& OutSuccess, EItemRarity Rarity = EItemRarity::EIR_Common);
	
	UFUNCTION(BlueprintCallable)
	void AddItemAsObject(UItemObject* InItem);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItem(TSubclassOf<UItemObject> InItemClass, int32 Amount);
	
	UFUNCTION(BlueprintCallable)
	void RemoveItemAsObject(UItemObject* InItem);
	
	UFUNCTION(BlueprintCallable)
	void SwapItems(int32 InSlot, int32 OutSlot);
	
	UFUNCTION(BlueprintCallable)
	void UpdateInventory();

	UFUNCTION(BlueprintCallable)
	void DEBUG_PrintSlots();
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsEmpty();
	
	UFUNCTION(BlueprintCallable)
	bool SaveInventoryToFile(const FString& FilePath) const;
	
	UFUNCTION(BlueprintCallable)
	bool LoadInventoryFromFile(const FString& FilePath);

private:
	
	UFUNCTION()
	bool HasNotFullSlotOfItem(const FName& ItemID, int32& OutIndex);
	UFUNCTION()
	bool FindFirstSlotOfType(int32& OutIndex, const FName InName = FName());

	UFUNCTION()
	bool CreateNewEmptySlotOfType(TSubclassOf<UItemObject> InItemClass);

	UFUNCTION()
	bool TryToFill(int32 InID, int32& AmountToAdd, int32& Overflow);
	
	UItemObject* CreateItemObject(TSubclassOf<UItemObject> InItemClass, int32 Amount, EItemRarity Rarity = EItemRarity::EIR_Common);
	
	bool AddItemToEmptySlot(UItemObject* InItem);
};
