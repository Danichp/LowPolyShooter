// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TMS_Spawner.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_LS_API ATMS_Spawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ATMS_Spawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<class UStaticMeshComponent> SMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Components)
	TObjectPtr<class USceneComponent> Scene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SpawnerData)
	TSubclassOf<AActor> ClassToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=SpawnerData)
	float SpawnDelay = 5.f;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Spawn();

	UFUNCTION()
	void ResetTimer(AActor* DestroyedActor);

private:
	FTimerHandle RespawnHandle;
};
