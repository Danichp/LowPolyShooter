// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_EnemyControllerBase.h"
#include "GameFramework/Character.h"
#include "TMS_AIPerception.h"
#include "TMS_EnemyCharacterBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TMS_LS/Utilities/TMS_AIPatrolPath.h"
#include "TMS_LS/Components/NPCConfigComponent.h"
#include "TMS_LS/Utilities/TMS_DeveloperSettings.h"


// Sets default values
ATMS_EnemyControllerBase::ATMS_EnemyControllerBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Perception = CreateDefaultSubobject<UTMS_AIPerception>("Perception");
}

FGenericTeamId ATMS_EnemyControllerBase::GetGenericTeamId() const
{
	return FGenericTeamId(static_cast<uint8>(CurrentTeamType));
}

ETeamAttitude::Type ATMS_EnemyControllerBase::GetTeamAttitudeTowards(const AActor& Other) const
{
	if (const APawn* OtherPawn = Cast<APawn>(&Other))
	{
		if (const IGenericTeamAgentInterface* OtherAgent =
			Cast<IGenericTeamAgentInterface>(OtherPawn->GetController()))
		{
			const FGenericTeamId& OtherTeamId = OtherAgent->GetGenericTeamId();
			ETeamType OtherTeamType = static_cast<ETeamType>(OtherTeamId.GetId());
			if (UTMS_DeveloperSettings::Get()->TeamsAttitudes.Find(CurrentTeamType) &&
				UTMS_DeveloperSettings::Get()->TeamsAttitudes[CurrentTeamType].
				TeamAttitude.Find(OtherTeamType))
			{
				return UTMS_DeveloperSettings::Get()->TeamsAttitudes[CurrentTeamType].TeamAttitude[OtherTeamType];
			}
		}
	}
	return ETeamAttitude::Neutral;
}

// Called when the game starts or when spawned
void ATMS_EnemyControllerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

AActor* ATMS_EnemyControllerBase::GetTargetActor()
{
	if (!GetBlackboardComponent()) return nullptr;
	return Cast<AActor>(GetBlackboardComponent()->GetValueAsObject(TargetEnemyKeyName));
}

FVector ATMS_EnemyControllerBase::GetTargetHeadBoneLocation()
{
	FVector DefaultLocation = GetBlackboardComponent()->GetValueAsVector("TargetLocation");
	if (!GetTargetActor()) return DefaultLocation;

	ACharacter* Char = Cast<ACharacter>(GetTargetActor());
	if (!Char) return DefaultLocation;

	return Char->GetMesh()->GetBoneLocation(FName("head"));
}

// Called every frame
void ATMS_EnemyControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//SetFocalPoint(GetTargetHeadBoneLocation());
	//SetFocus(GetTargetActor());
	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetPawn()->GetActorLocation(),
		GetTargetHeadBoneLocation());
	SetControlRotation(TargetRotation);
}

void ATMS_EnemyControllerBase::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (ATMS_EnemyCharacterBase* EnemyPawn = Cast<ATMS_EnemyCharacterBase>(InPawn))
	{
		CurrentTeamType = EnemyPawn->NPCConfig->GetTeamType();
		if (auto BT = EnemyPawn->NPCConfig->GetDefaultBT())
		{
			RunBehaviorTree(BT);
			GetBlackboardComponent()->SetValueAsObject(PatrolPathKeyName, EnemyPawn->PatrolPath.Get());
		}
	}
}

