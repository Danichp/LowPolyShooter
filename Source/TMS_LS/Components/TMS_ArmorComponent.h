// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TMS_LS/Items/Equipment/EquipmentConstuctor.h"
#include "TMS_ArmorComponent.generated.h"

DECLARE_DELEGATE(FOnEquipmentChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API UTMS_ArmorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTMS_ArmorComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Setup")
	TObjectPtr<UEquipmentConstuctor> CurrentEquipment;
	
	FOnEquipmentChanged OnEquipmentChanged;
	
protected:
	virtual void BeginPlay() override;

public:
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UEquipmentConstuctor* GetCurrentEquipment() const { return CurrentEquipment; }
	
	UFUNCTION(BlueprintCallable)
	void SetEquipment(UEquipmentConstuctor* Equipment);
	
	
};
