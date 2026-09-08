// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDataTypes.h"
#include "Actors/ItemEquipment.h"
#include "Components/ActorComponent.h"
#include "TMS_EquipmentComponent.generated.h"

class UItemObject;

USTRUCT(BlueprintType)
struct FEquipmentProcess
{
	GENERATED_BODY()
	
	FEquipmentProcess() {};
	FEquipmentProcess(const FEquipmentProcess& other)
	{
		ItemData = other.ItemData;
		Slot = other.Slot;
		bEquipment = other.bEquipment;
	};
	FEquipmentProcess(UItemObject* InItem, EEquipmentType InSlot, bool bInEquipment)
	{
		ItemData = InItem;
		Slot = InSlot;
		bEquipment = bInEquipment;
	}

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UItemObject* ItemData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EEquipmentType Slot = EEquipmentType::EET_MAX;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bEquipment = false;

	bool IsValid()
	{
		return Slot != EEquipmentType::EET_MAX;
	}
	
	bool operator==(const FEquipmentProcess& Other) const
	{
		return Slot == Other.Slot;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginEquipSignature, EEquipmentType, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBeginUnEquipSignature, EEquipmentType, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishEquipSignature, EEquipmentType, Slot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinishUnequipSignature, EEquipmentType, Slot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUpdateSignature, EEquipmentType, Slot);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentUpdatedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnEquipmentInitializedSignature);

UCLASS(BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API UTMS_EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTMS_EquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentUpdatedSignature OnEquipmentUpdated;
	UPROPERTY(BlueprintAssignable)
	FOnEquipmentInitializedSignature OnEquipmentInitialized;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EEquipmentType, TObjectPtr<UItemObject>> EquipmentObjects;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EEquipmentType, TObjectPtr<AItemEquipment>> EquipmentActors;

	UPROPERTY(BlueprintAssignable)
	FOnBeginEquipSignature OnBeginEquip;
	UPROPERTY(BlueprintAssignable)
	FOnBeginUnEquipSignature OnBeginUnequip;
	UPROPERTY(BlueprintAssignable)
	FOnFinishEquipSignature OnFinishEquip;
	UPROPERTY(BlueprintAssignable)
	FOnFinishUnequipSignature OnFinishUnequip;

	UPROPERTY(BlueprintAssignable)
	FOnSlotUpdateSignature OnSlotUpdate;
private:
	FTimerHandle EquipmentHandle;

	TQueue<FEquipmentProcess> EquipmentProcesses;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FEquipmentProcess CurrentEquipmentProcess;

public:
	UFUNCTION(BlueprintCallable)
	void AddPendingEquipment(const FEquipmentProcess& InEP);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetEquipmentBySlot(EEquipmentType InSlot,
		UItemObject*& OutItem,
		AItemEquipment*& OutEquipmentActor);
	
	// UFUNCTION(BlueprintCallable)
	// bool SaveEquipmentToFile(const FString& FilePath) const;
	//
	// UFUNCTION(BlueprintCallable)
	// bool LoadEquipmentFromFile(const FString& FilePath);

private:
	void NextPendingEquipment();

	UFUNCTION()
	void EquipSlot(EEquipmentType InSlot, UItemObject* InItem);
	UFUNCTION()
	void UnequipSlot(EEquipmentType InSlot);

	UFUNCTION()
	void ProcessEquip(EEquipmentType InSlot);
	UFUNCTION()
	void ProcessUnequip(EEquipmentType InSlot);

	UFUNCTION()
	void FinishEquip();
	UFUNCTION()
	void FinishUnequip();

	void MoveCurrentItemInInventory();
	
	UFUNCTION()
	void DestroyedEquipment();
	
	// FString SerializeToJson() const;
	//
	// bool DeserializeFromJson(const FString& InJsonString);
};
