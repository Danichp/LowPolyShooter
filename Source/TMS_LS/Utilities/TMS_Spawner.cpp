// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_Spawner.h"

ATMS_Spawner::ATMS_Spawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SMesh->SetupAttachment(GetRootComponent());

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(SMesh);
	Scene->SetRelativeLocation(FVector(0, 0, 50.f));
	// ...
}


// Called when the game starts
void ATMS_Spawner::BeginPlay()
{
	Super::BeginPlay();
	Spawn();
	// ...
	
}

void ATMS_Spawner::Spawn()
{
	if (ClassToSpawn)
	{
		AActor* NewActor = GetWorld()->SpawnActor(ClassToSpawn.Get());
		if (!NewActor) return;
		NewActor->SetActorLocation(Scene->GetComponentLocation());
		NewActor->OnDestroyed.AddDynamic(this, &ATMS_Spawner::ResetTimer);
	}
}

void ATMS_Spawner::ResetTimer(AActor* DestroyedActor)
{
	if (IsValid(DestroyedActor))
	{
		DestroyedActor->OnDestroyed.RemoveDynamic(this, &ATMS_Spawner::ResetTimer);
	}
	GetWorldTimerManager().SetTimer(RespawnHandle, this, &ATMS_Spawner::Spawn, SpawnDelay, false);
}
