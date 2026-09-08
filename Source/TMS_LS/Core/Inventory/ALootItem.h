// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemObjects/ItemObject.h"
#include "TMS_LS/Core/TMS_InteractInterface.h"
#include "ALootItem.generated.h"

UCLASS()
class TMS_LS_API AALootItem : public AActor, public ITMS_InteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AALootItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UItemObject> HoldingItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UItemObject> ItemClass;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void InitLoot();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool TryInteract(TWeakObjectPtr<class APlayerController> InPC) override;
};
