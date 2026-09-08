// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TMS_LS/Core/TMS_BaseCharacter.h"
#include "TMS_LS/Core/TMS_InteractInterface.h"
#include "TMS_EnemyCharacterBase.generated.h"

class UNPCConfigComponent;
class UTMS_LootComponent;
class ATMS_AIPatrolPath;

UCLASS()
class TMS_LS_API ATMS_EnemyCharacterBase : public ATMS_BaseCharacter, public ITMS_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATMS_EnemyCharacterBase();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI")
	TSoftObjectPtr<ATMS_AIPatrolPath> PatrolPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Loot")
	TObjectPtr<UTMS_LootComponent> LootComponent;
	
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="NPC")
	TObjectPtr<UNPCConfigComponent> NPCConfig;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	void OnInventoryUpdated();
	virtual void OnDeath() override;

	virtual bool TryInteract(TWeakObjectPtr<class APlayerController> InPC) override;
};
