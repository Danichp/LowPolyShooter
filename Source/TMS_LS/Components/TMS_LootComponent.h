// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TMS_LootComponent.generated.h"


class UItemObject;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API UTMS_LootComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTMS_LootComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Loot")
	TArray<TSubclassOf<UItemObject>> PossibleLoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Loot", meta = (UIMin = 0, ClampMin = 0, UIMax = 255, ClampMax = 255))
	uint8 LootItemsAmount = 0;
#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	virtual void BeginPlay() override;
protected:


public:

	void GenerateLoot();	
};
