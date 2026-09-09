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
	AActor* Target = GetTargetActor();
	if (!Target)
	{
		return FVector::ZeroVector;
	}

	ACharacter* Char = Cast<ACharacter>(Target);
	if (!Char)
	{
		return Target->GetActorLocation();
	}

	USkeletalMeshComponent* Mesh = Char->GetMesh();
	if (!Mesh) return Target->GetActorLocation();

	const FName HeadBoneName = TEXT("ead");
	int32 BoneIndex = Mesh->GetBoneIndex(HeadBoneName);
	if (BoneIndex == INDEX_NONE)
	{
		return Target->GetActorLocation();
	}

	return Mesh->GetBoneLocation(HeadBoneName);
}

void ATMS_EnemyControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector TargetPos = GetTargetHeadBoneLocation();
	if (TargetPos.IsNearlyZero())
	{
		return;
	}

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
		GetPawn()->GetActorLocation(),
		TargetPos
	);
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
		
		if (UTMS_HealthComponent* Health = EnemyPawn->GetComponentByClass<UTMS_HealthComponent>())
		{
			Health->OnDamaged.AddDynamic(this, &ATMS_EnemyControllerBase::HandleDamaged);
		}
	}
}

void ATMS_EnemyControllerBase::HandleDamaged(AActor* DamagedActor, float Damage, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!InstigatedBy) return;
	APawn* InstigatorPawn = InstigatedBy->GetPawn();
	if (!InstigatorPawn) return;

	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!BB) return;

	BB->SetValueAsObject(TargetEnemyKeyName, InstigatorPawn);
}

