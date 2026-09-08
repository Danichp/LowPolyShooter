#pragma once

#include "CoreMinimal.h"
#include "ItemDataTypes.generated.h"

class AItemActor;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor=true))
enum class ELootType : uint8
{
	ELT_None = 0 UMETA(Hidden),
	ELT_Weapon = 1 << 0 UMETA(DisplayName = "Weapon"),
	ELT_Ammo = 1 << 1 UMETA(DisplayName = "Ammo"),
	ELT_Health = 1 << 2 UMETA(DisplayName = "Health"),
};

UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	EET_Armor,
	EET_Body,
	EET_Legs,
	EET_Main,
	EET_Secondary,
	EET_Amulet,
	EET_MAX UMETA(Hidden),
};

ENUM_RANGE_BY_COUNT(EEquipmentType, EEquipmentType::EET_MAX);

UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Common,
	EIR_Rare,
	EIR_Legendary,
	EIR_MAX UMETA(Hidden),
};

UENUM(BlueprintType)
enum class EEquipmentAction : uint8
{
	EEA_Equip,
	EEA_Unequip,
	EEA_Look,
	EEA_Idle,
	EEA_MAX UMETA(Hidden),
};

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UStaticMesh> PreviewMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AItemActor> ItemClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ELootType Type = ELootType::ELT_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmount = 64;
};