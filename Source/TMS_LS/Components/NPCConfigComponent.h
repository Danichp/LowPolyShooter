// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TMS_LS/Core/AI/TMS_EnemyControllerBase.h"
#include "TMS_LS/Core/Data/TMS_DataTypes.h"
#include "TMS_LS/Core/Inventory/ItemDataTypes.h"
#include "NPCConfigComponent.generated.h"


class UBehaviorTree;
class UItemObject;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API UNPCConfigComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNPCConfigComponent();
	
	TObjectPtr<UBehaviorTree> GetDefaultBT() {return DefaultBT;}
	
	ETeamType GetTeamType() const { return TeamType; };

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TEnumAsByte<ETeamType> TeamType = ETeamType::ETT_Bandits;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EEquipmentType, TSubclassOf<UItemObject>> DefaultEquipment;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UBehaviorTree> DefaultBT;
	
};