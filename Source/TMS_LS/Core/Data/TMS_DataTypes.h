#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "TMS_DataTypes.generated.h"


#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarDebugMode(
	TEXT("tms_cheats.Debug"), 0, TEXT("0 for false, >0 for true"));
#endif

USTRUCT(Blueprintable, BlueprintType)
struct FTMS_AnimData
{
	GENERATED_BODY()

	FTMS_AnimData()
	{
		
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> GetUpMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> ClimbMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> JumpDownMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> VaultMontage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> DeathMontage = nullptr;

	bool IsValid() const
	{
		return (GetUpMontage && ClimbMontage && JumpDownMontage && VaultMontage && DeathMontage);
	}
};

UENUM(BlueprintType)
enum class EWeaponActionType : uint8
{
	EWAT_Main,
	EWAT_Secondary,
	EWAT_Reload,
	EWAT_MAX UMETA(Hidden)
};
UENUM(BlueprintType)
enum ETeamType : uint8
{
	ETT_Player,
	ETT_NPC,
	ETT_Bandits,
	ETT_Mercenaries,
	ETT_MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FTeamAffiliation
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<TEnumAsByte<ETeamType>, TEnumAsByte<ETeamAttitude::Type>> TeamAttitude;
};


USTRUCT(BlueprintType)
struct FWeaponAnimData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> CAnim;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAnimMontage> WAnim;
	
};

USTRUCT(BlueprintType)
struct FWeaponRuntimeStats
{
	GENERATED_BODY()
	
	FWeaponRuntimeStats() : Damage(0), FireRate(0),
	MaxAmmo(0), EffectiveRange(0), AimFOV(0), AimSpeed(0)
	{};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D Recoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectiveRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D AccuracyAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2D AccuracyAimAngle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimFOV;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AimSpeed;
};


USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	FText  LevelName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TSoftObjectPtr<UTexture2D> PreviewImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Level")
	TSoftObjectPtr<UWorld> LevelMap;
	
};

