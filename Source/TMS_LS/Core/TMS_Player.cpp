// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "TMS_InteractInterface.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TMS_LS/Components/TMS_ArmorComponent.h"
#include "TMS_LS/Components/TMS_WeaponComponent.h"

ATMS_Player::ATMS_Player()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	SpringArm->TargetArmLength = 250.f;
	SpringArm->SocketOffset = FVector(0.f, 100.f, 50.f);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	MotionWarper = CreateDefaultSubobject<UMotionWarpingComponent>("MotionWarper");
	
	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HelmetMesh");
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetLeaderPoseComponent(GetMesh());
	
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyMesh");
	BodyMesh->SetupAttachment(GetMesh());
	BodyMesh->SetLeaderPoseComponent(GetMesh());
	
	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HandsMesh");
	HandsMesh->SetupAttachment(GetMesh());
	HandsMesh->SetLeaderPoseComponent(GetMesh());
	
	LegsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LegsMesh");
	LegsMesh->SetupAttachment(GetMesh());
	LegsMesh->SetLeaderPoseComponent(GetMesh());
	
	BootsMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BootsMesh");
	BootsMesh->SetupAttachment(GetMesh());
	BootsMesh->SetLeaderPoseComponent(GetMesh());
	
	
	
	TargetFOV = Camera->FieldOfView;
}

void ATMS_Player::OnInventoryUpdated()
{
	if (InventoryComponent)
	{
		FString InventorySavePath = FPaths::ProjectSavedDir() + TEXT("PlayerData/Inventory.json");
		InventoryComponent->SaveInventoryToFile(InventorySavePath);
	}
}

void ATMS_Player::OnEquipmentUpdated()
{
	// if (EquipmentComponent)
	// {
	// 	FString EquipmentSavePath = FPaths::ProjectSavedDir() + TEXT("PlayerData/Equipment.json");
	// 	EquipmentComponent->SaveEquipmentToFile(EquipmentSavePath);
	// }
}

void ATMS_Player::BeginPlay()
{
	Super::BeginPlay();
	CurrentFOV = TargetFOV;

	if (WeaponComponent)
	{
		WeaponComponent->OnAim.AddDynamic(this, &ATMS_Player::OnAimUpdate);
	}
	
	if (InventoryComponent)
	{
		FString InventoryLoadPath = FPaths::ProjectSavedDir() + TEXT("PlayerData/Inventory.json");
		InventoryComponent->LoadInventoryFromFile(InventoryLoadPath);
		
		InventoryComponent->OnInventoryUpdated.AddDynamic(this, &ATMS_Player::OnInventoryUpdated);
	}
	
	// if (EquipmentComponent)
	// {
	// 	FString EquipmentLoadPath = FPaths::ProjectSavedDir() + TEXT("PlayerData/Equipment.json");
	// 	EquipmentComponent->LoadEquipmentFromFile(EquipmentLoadPath);
	// 	
	// 	EquipmentComponent->OnEquipmentUpdated.AddDynamic(this, &ATMS_Player::OnEquipmentUpdated);
	// }
	
	PPC = Cast<APlayerController>(GetController());
	if (!PPC) return;

	PHUD = Cast<ATMS_HUD>(PPC->GetHUD());
	if (!PHUD) return;
}

void ATMS_Player::OnEquipmentChanged()
{
	Super::OnEquipmentChanged();
	if (UEquipmentConstuctor* EC = ArmorComponent->GetCurrentEquipment())
	{
		for (const auto& Pair : EC->MeshesByLayer)
		{
			if (USkeletalMeshComponent* SkMC = GetSkMByEquipmentLayer(Pair.Key))
			{
				if (USkeletalMesh* SkM = Pair.Value.LoadSynchronous())
				{
					SkMC->SetSkeletalMeshAsset(SkM);
					SkMC->SetLeaderPoseComponent(GetMesh(), true);
				}
			}
		}
	}
}

USkeletalMeshComponent* ATMS_Player::GetSkMByEquipmentLayer(EEquipmentLayer Layer)
{
	switch (Layer)
	{
	case EEquipmentLayer::EEL_Helmet:
		return HelmetMesh;
	case EEquipmentLayer::EEL_Head:
		break;
	case EEquipmentLayer::EEL_Body:
		return BodyMesh;
	case EEquipmentLayer::EEL_Hands:
		return HandsMesh;
	case EEquipmentLayer::EEL_Legs:
		return LegsMesh;
	case EEquipmentLayer::EEL_Boots:
		return BootsMesh;
	case EEquipmentLayer::EEL_MAX:
		break;
	}
	return nullptr;
}

void ATMS_Player::MathTick(float DeltaTime)
{
	// =, -, *, / , %
	// FMath::
	const FVector StartPoint = GetActorLocation();
	const FVector EndPoint = FMath::VRandCone(GetActorForwardVector(), 15.f) * FMath::FRandRange(500.f, 1000.f);
	const FVector Point = GetActorForwardVector() * FMath::FRandRange(600.F, 800.F);
	const FVector Closest = FMath::ClosestPointOnLine(StartPoint, EndPoint, Point);
	
	UKismetSystemLibrary::DrawDebugLine(this, StartPoint, EndPoint, FLinearColor::Blue, 0, 2.f);
	UKismetSystemLibrary::DrawDebugLine(this, StartPoint, Point, FLinearColor::Green, 0, 2.f);
	UKismetSystemLibrary::DrawDebugLine(this, Closest, Point, FLinearColor::Red, 0, 2.f);
	
	UKismetSystemLibrary::DrawDebugPoint(this, Closest, 5.f, FLinearColor::Yellow, 0);
	
	FVector RandVector = FMath::VRand()*FMath::RandRange(300, 500);
	FVector RandScale = FMath::VRand()*FMath::RandRange(0.5f, 2.f);
	FRotator RandRotator = FMath::VRand().Rotation();
	
	FTransform RandTransform = FTransform(RandRotator, RandVector, RandScale);
	FMatrix Matrix = RandTransform.ToMatrixWithScale();
	
	UE_LOG(LogTemp, Display, TEXT("Transform: %s, \n Matrix: %s"), *RandTransform.ToHumanReadableString(), *Matrix.ToString() );
	
}

void ATMS_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetCharacterMovement()->IsWalking() && CurrentJumpBufferTime > 0.f)
	{
		Jump();
		CurrentFallBufferTime = 0;
	}
	if (GetCharacterMovement()->IsFalling() && GetCharacterMovement()->Velocity.Z < 0)
	{
		CurrentFallBufferTime+=DeltaTime;
	}
	CurrentJumpBufferTime-=DeltaTime;
	FOV_Update(DeltaTime);
	
	CheckInteractable();
	
	//MathTick(DeltaTime);
}

float ATMS_Player::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
#if !UE_BUILD_SHIPPING
	if (CVarPlayerGodMode.GetValueOnGameThread() > 0) return 0;
#endif
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ATMS_Player::MainInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent || !WeaponComponent->CurrentWeapon) return;
	bool bUseItem = InputActionValue.Get<bool>();
	
	WeaponComponent->UseWeapon(EWeaponActionType::EWAT_Main, bUseItem);
}

void ATMS_Player::SecondaryInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent || !WeaponComponent->CurrentWeapon) return;
	bool bUseItem = InputActionValue.Get<bool>();

	WeaponComponent->UseWeapon(EWeaponActionType::EWAT_Secondary, bUseItem);
}

void ATMS_Player::ReloadInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent || !WeaponComponent->CurrentWeapon) return;

	WeaponComponent->UseWeapon(EWeaponActionType::EWAT_Reload);
	
}

void ATMS_Player::CheckInteractable()
{
	TArray<FHitResult> Hits;
	FVector StartLocation = Camera->GetComponentLocation();
	FVector Direction = Camera->GetForwardVector();
	FVector EndLocation = Camera->GetComponentLocation() + Direction * 500.f;

	TArray<AActor*> IgnoreActors;
	IgnoreActors.Add(GetOwner());
	
	
	UKismetSystemLibrary::CapsuleTraceMulti(GetWorld(), StartLocation, EndLocation,
		10.f, 10.f, UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false, IgnoreActors, EDrawDebugTrace::Type::None, Hits, true);

	if (Hits.Num() <= 0) return;
	
	FHitResult Hit = Hits[0];
	
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		if (ITMS_InteractInterface* HitActor = Cast<ITMS_InteractInterface>(Hit.GetActor()))
		{
			InteractActor = Hit.GetActor();
		}
	}
}

void ATMS_Player::OnKill(AActor* DamagedActor)
{
	OnKilled.Broadcast(DamagedActor);
}

void ATMS_Player::OnHit(AActor* DamagedActor, float Damage)
{
	OnDamaged.Broadcast(DamagedActor, Damage);
}

void ATMS_Player::OnAimUpdate(bool bNewActive)
{
	if (!WeaponComponent || !WeaponComponent->GetFireWeapon()) return;
	SetTargetFOV(bNewActive ? WeaponComponent->GetFireWeapon()->AimingFOV : 90.f);
}

void ATMS_Player::FOV_Update(float DeltaTime)
{
	float AimSpeed = 2.f;
	if (WeaponComponent && WeaponComponent->GetFireWeapon())
	{
		AimSpeed = WeaponComponent->GetFireWeapon()->FOV_InterpSpeed;
	}
	float NewFOV = FMath::FInterpTo(CurrentFOV, TargetFOV, DeltaTime, AimSpeed);

	if (!FMath::IsNearlyEqual(NewFOV, TargetFOV))
	{
		CurrentFOV = NewFOV;
		Camera->SetFieldOfView(CurrentFOV);
	}
}

void ATMS_Player::SetTargetFOV(float NewTarget)
{
	TargetFOV = NewTarget;
}

void ATMS_Player::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (!InputContext || !IsValid(InputData)) return;

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController) return;

	UEnhancedInputLocalPlayerSubsystem* EIS = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		PlayerController->GetLocalPlayer());
	if (!EIS) return;

	EIS->ClearAllMappings();
	EIS->AddMappingContext(InputContext, 100);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) return;

	EIC->BindAction(InputData->MoveInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnMoveInput);
	EIC->BindAction(InputData->MoveInput, ETriggerEvent::Completed, this, &ATMS_Player::StopMoveInput);
	EIC->BindAction(InputData->LookInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnLookInput);
	EIC->BindAction(InputData->SprintInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnSprintInput);
	EIC->BindAction(InputData->CrouchInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnCrouchInput);

	EIC->BindAction(InputData->JumpInput, ETriggerEvent::Started, this, &ATMS_Player::Jump);
	EIC->BindAction(InputData->JumpInput, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

	EIC->BindAction(InputData->MainInput, ETriggerEvent::Triggered, this, &ATMS_Player::MainInput);
	EIC->BindAction(InputData->SecondaryInput, ETriggerEvent::Triggered, this, &ATMS_Player::SecondaryInput);
	EIC->BindAction(InputData->ReloadInput, ETriggerEvent::Triggered, this, &ATMS_Player::ReloadInput);

	EIC->BindAction(InputData->InventoryInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnInventoryInput);
	EIC->BindAction(InputData->StatsInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnStatsInput);
	EIC->BindAction(InputData->InteractInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnInteractInput);

	EIC->BindAction(InputData->PauseInput, ETriggerEvent::Triggered, this, &ATMS_Player::OnPauseInput);
}

bool ATMS_Player::CanJumpInternal_Implementation() const
{
	if (CurrentFallBufferTime > 0 && CurrentFallBufferTime < DefaultJumpBufferTime)
	{
		return !bIsCrouched;
	}
	return Super::CanJumpInternal_Implementation();
}

void ATMS_Player::OnMoveInput(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnMoveInput : %s"), *Value.ToString());
	MoveInput = Value.Get<FVector2D>().GetSafeNormal();
	float InputScale = MoveInput.Length();
	

	FVector Forward = GetControlRotation().Vector().GetSafeNormal2D();
	FVector Right = Forward.Cross(FVector::UpVector);

	FVector Dir = Forward * MoveInput.X + Right * MoveInput.Y;

	AddMovementInput(Dir, InputScale);
	GetCharacterMovement()->bUseControllerDesiredRotation = MoveInput.Length() > 0.f;
	bUseControllerRotationYaw = MoveInput.Length() == 0.f;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
}

void ATMS_Player::StopMoveInput(const FInputActionValue& Value)
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ATMS_Player::OnLookInput(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnLookInput : %s"), *Value.ToString());
	FVector2D LookInput = Value.Get<FVector2D>();

	AddControllerPitchInput(LookInput.Y);
	AddControllerYawInput(LookInput.X);
}

void ATMS_Player::OnSprintInput(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnSprintInput : %s"), *Value.ToString());
	bool ToSprint = Value.Get<bool>();

	if (WeaponComponent)
	{
		WeaponComponent->UseWeapon(EWeaponActionType::EWAT_Secondary, false);
	}

	if (HealthComponent)
	{
		HealthComponent->SprintInput(ToSprint);
	}
}

void ATMS_Player::OnCrouchInput(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Display, TEXT("OnCrouchInput : %s"), *Value.ToString());
	if (!CanCrouch()) return;
	
 	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	if (!MovementComponent) return;
	
	if (MovementComponent->IsSwimming())
	{
		AddMovementInput(FVector::DownVector);
	}
	
	MovementComponent->IsCrouching() ? MovementComponent->Crouch() : MovementComponent->UnCrouch();
	bCrouching = MovementComponent->IsCrouching();
}

void ATMS_Player::OnPauseInput()
{
	if (!PHUD) return;

	switch (PHUD->GetUIState())
	{
	case EUIState::EUIS_Game :
		PHUD->SetUIState(EUIState::EUIS_Pause);
		break;
	case EUIState::EUIS_Pause:
		break;
	case EUIState::EUIS_Loot:
	case EUIState::EUIS_Equipment:
		PHUD->SetUIState(EUIState::EUIS_Game);
		break;
	}	
}
void ATMS_Player::OnInventoryInput(const FInputActionValue& Value)
{
	if (!PHUD) return;

	switch (PHUD->GetUIState())
	{
	case EUIState::EUIS_Game :
		PHUD->SetUIState(EUIState::EUIS_Equipment);
		break;
	case EUIState::EUIS_Pause:
		break;
	case EUIState::EUIS_Loot:
	case EUIState::EUIS_Score:
	case EUIState::EUIS_Equipment:
		PHUD->SetUIState(EUIState::EUIS_Game);
		break;
	}
}

void ATMS_Player::OnStatsInput(const FInputActionValue& Value)
{
	if (!PHUD) return;

	switch (PHUD->GetUIState())
	{
	case EUIState::EUIS_Game :
		PHUD->SetUIState(EUIState::EUIS_Score);
		break;
	case EUIState::EUIS_Pause:
		break;
	case EUIState::EUIS_Loot:
	case EUIState::EUIS_Score:
		PHUD->SetUIState(EUIState::EUIS_Game);
		break;
	case EUIState::EUIS_Equipment:
		break;
	}
}

void ATMS_Player::OnInteractInput(const FInputActionValue& Value)
{
	if (!GetWorld()) return;
	if (ITMS_InteractInterface* Interactable = Cast<ITMS_InteractInterface>(InteractActor))
	{
		Interactable->TryInteract(PPC);
	}
}

void ATMS_Player::Jump()
{
	if (CanVault() && TryVault()) return;
	CurrentJumpBufferTime = DefaultJumpBufferTime;
	Super::Jump();
}

bool ATMS_Player::CanVault() const
{
	return !bIsVaulting;
}

bool ATMS_Player::TryVault()
{
	if (!GetWorld() || !VaultAnims.IsValid()) return false;
	UCapsuleComponent* Capsule = GetCapsuleComponent();
	if (!Capsule) return false;
	float TraceRadius = Capsule->GetScaledCapsuleRadius() / 2.f;

	bool bWallThick = false;
	bool bWallHigh = false;
	
	FHitResult Hit;
	FVector StartLocation = GetActorLocation() + (FVector::DownVector * 55.f);
	FVector EndLocation = StartLocation + GetActorForwardVector() * 100.f;
	TArray<AActor*> IgnoreActors;
	// Поиск препятствия перед собой
	GetWorld()->LineTraceSingleByChannel(Hit,
		StartLocation, EndLocation, ECC_GameTraceChannel1);

	if (!Hit.bBlockingHit) return false;

	FVector HitLocation = Hit.Location;
	FVector HitNormal = Hit.Normal;

	EndLocation = HitLocation + HitNormal * (-10.f);
	StartLocation = EndLocation + FVector(0, 0, Capsule->GetScaledCapsuleHalfHeight() * 3.f);

	// Проверка, можем ли мы перелезть
	GetWorld()->LineTraceSingleByChannel(Hit,
		StartLocation, EndLocation, ECC_GameTraceChannel1);

	if (!Hit.bBlockingHit || Hit.bStartPenetrating) return false;

	FVector WallPeakLocation = Hit.Location;
	bWallHigh = WallPeakLocation.Z - HitLocation.Z > 80.f;

	EndLocation = HitLocation + HitNormal * (-50.f);
	StartLocation = EndLocation + FVector(0, 0, Capsule->GetScaledCapsuleHalfHeight() * 4.f);

	// Проверка на толщину препятствия
	GetWorld()->LineTraceSingleByChannel(Hit,
		StartLocation, EndLocation, ECC_GameTraceChannel1);

	if (Hit.bBlockingHit)
	{
		bWallThick = WallPeakLocation.Z - Hit.Location.Z < 30.f;
	}

	// TODO если успеем, добавить Motion Warping
	MotionWarper->AddOrUpdateWarpTargetFromLocationAndRotation("Vault", WallPeakLocation, GetActorRotation());

	bIsVaulting = true;
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	if (!bWallHigh)
	{

		FVector TargetLocation = bWallThick ? GetActorLocation() + HitNormal * 50.f : FVector(GetActorLocation().X, GetActorLocation().Y, WallPeakLocation.Z - 10.f);
		SetActorLocation(TargetLocation);
		float AnimDuration = PlayAnimMontage(bWallThick ? VaultAnims.GetUpMontage : VaultAnims.VaultMontage);
		FTimerHandle AnimHandle;
		GetWorldTimerManager().SetTimer(AnimHandle, this, &ATMS_Player::FinishVault, AnimDuration * 0.8f, false);
		return true;
	}
	else
	{
		FVector TargetLocation = FVector(GetActorLocation().X, GetActorLocation().Y, WallPeakLocation.Z-20.f);
		SetActorLocation(TargetLocation);
		float AnimDuration = PlayAnimMontage(VaultAnims.ClimbMontage);
		FTimerHandle AnimHandle;
		FTimerDelegate AnimDelegate = FTimerDelegate::CreateLambda([this, bWallThick]
		{
			if (bWallThick)
			{
				this->FinishVault();
			}
			else
			{
				float AnimDuration = PlayAnimMontage(VaultAnims.JumpDownMontage);
				FTimerHandle AnimHandle;
				GetWorldTimerManager().SetTimer(AnimHandle,
					this, &ATMS_Player::FinishVault, AnimDuration * 0.5f, false);
			}
		});
		GetWorld()->GetTimerManager().SetTimer(AnimHandle, AnimDelegate, AnimDuration, false);
		return true;
	}
	return false;
}

void ATMS_Player::FinishVault()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	bIsVaulting = false;
}
