// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageZone.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ADamageZone::ADamageZone()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");

	SetRootComponent(Root);
}

// Called when the game starts or when spawned
void ADamageZone::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DamageHandle;
	GetWorldTimerManager().SetTimer(DamageHandle, this, &ThisClass::DoDamage, DamageDelay, true);
}

// Called every frame
void ADamageZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), Radius, 30, Color);
}

void ADamageZone::DoDamage()
{
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage,
		GetActorLocation(), Radius, DamageType, {},
		this, nullptr, false);
}
