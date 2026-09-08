// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemActor.h"
#include "TMS_LS/Core/Inventory/ItemDataTypes.h"
#include "ItemEquipment.generated.h"

UCLASS()
class TMS_LS_API AItemEquipment : public AItemActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItemEquipment();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EEquipmentType EquipmentType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	TMap<EEquipmentAction, UAnimMontage*> ActionMontages;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UAnimMontage* GetMontageByType(EEquipmentAction InAction);
};
