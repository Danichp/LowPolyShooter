// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_Main.h"

#include "TMS_Pause.h"
#include "Kismet/GameplayStatics.h"
#include "TMS_LS/Core/TMS_Player.h"

void UTMS_Main::OnKilled_Implementation(AActor* KilledActor)
{
	if (ATMS_Player* Player = Cast<ATMS_Player>(GetOwningPlayer()->GetPawn()))
	{
		UE_LOG(LogTemp, Display, TEXT("%s killed %s"), *Player->GetName(), *KilledActor->GetName());
	}
}

void UTMS_Main::OnDamaged_Implementation(AActor* DamagedActor, float Damage)
{
	if (ATMS_Player* Player = Cast<ATMS_Player>(GetOwningPlayer()->GetPawn()))
	{
		UE_LOG(LogTemp, Display, TEXT("%s hit %s for %f damage"), *Player->GetName(), *DamagedActor->GetName(), Damage);
	}
}

void UTMS_Main::NativeConstruct()
{
	Super::NativeConstruct();
	check(AttributePanel);

	if (ATMS_HUD* HUD = Cast<ATMS_HUD>(GetOwningPlayer()->GetHUD()))
	{
		HUD->OnUIStateChanged.AddDynamic(this, &UTMS_Main::OnUIStateChanged);
	}
	
	if (ATMS_Player* Player = Cast<ATMS_Player>(GetOwningPlayer()->GetPawn()))
	{
		Player->OnKilled.AddDynamic(this, &UTMS_Main::OnKilled);
		Player->OnDamaged.AddDynamic(this, &UTMS_Main::OnDamaged);
	}
	
}

void UTMS_Main::OnUIStateChanged_Implementation(EUIState InNewState)
{
	CurrentUIState = InNewState;

	UGameplayStatics::SetGamePaused(GetWorld(), false);
	if (PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
	}
	
	switch (CurrentUIState)
	{
	case EUIState::EUIS_Game:
		{
			FInputModeGameOnly InputMode;
			GetOwningPlayer()->SetInputMode(InputMode);
			GetOwningPlayer()->SetShowMouseCursor(false);
			break;
		}
	case EUIState::EUIS_Pause:
		if (IsValid(PauseWidgetClass))
		{
			PauseWidget = Cast<UTMS_Pause>(CreateWidget(GetOwningPlayer(), PauseWidgetClass));
			if (PauseWidget)
			{
				PauseWidget->AddToViewport(1);
				UGameplayStatics::SetGamePaused(GetWorld(), true);
			}
		}
	case EUIState::EUIS_Loot:
	case EUIState::EUIS_Equipment:
		{
			FInputModeUIOnly InputMode;
			InputMode.SetWidgetToFocus(this->TakeWidget());
			GetOwningPlayer()->SetInputMode(InputMode);
			GetOwningPlayer()->SetShowMouseCursor(true);
			this->SetKeyboardFocus();
			break;
		}
	case EUIState::EUIS_Score:
		{
			FInputModeGameOnly InputMode;
			GetOwningPlayer()->SetInputMode(InputMode);
			GetOwningPlayer()->SetShowMouseCursor(false);
			break;
		}
	}
}
