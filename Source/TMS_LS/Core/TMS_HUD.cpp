// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_HUD.h"
#include "Engine/Canvas.h"

void ATMS_HUD::DrawHUD()
{
	Super::DrawHUD();
	//DrawCrosshair();
}

void ATMS_HUD::PostRender()
{
	Super::PostRender();
	
	for (const auto& widget : HUD_Widgets)
	{
		widget->SetVisibility(bShowHUD ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	}
}

void ATMS_HUD::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(HUD_Widget.Get()))
	{
		if (UUserWidget* WHUD =
			CreateWidget(GetOwningPlayerController(), HUD_Widget, "HUD_Main"))
		{
			WHUD->AddToViewport();
			HUD_Widgets.AddUnique(WHUD);
		}
	}
}

void ATMS_HUD::SetUIState(EUIState InState)
{
	CurrentUIState = InState;
	
	OnUIStateChanged.Broadcast(CurrentUIState);
}

void ATMS_HUD::DrawCrosshair()
{
	int32 SizeX = Canvas->SizeX;
	int32 SizeY = Canvas->SizeY;
	FVector2D Center = FVector2D(SizeX / 2, SizeY / 2);

	const float LineSize = 10.0f;
	const float LineWidth = 2.f;
	const FLinearColor LineColor = FLinearColor::Green;

	DrawLine(Center.X + LineSize/2, Center.Y,
		Center.X + LineSize*2, Center.Y,
		LineColor, LineWidth);

	DrawLine(Center.X - LineSize/2, Center.Y,
		Center.X - LineSize*2, Center.Y,
		LineColor, LineWidth);

	DrawLine(Center.X , Center.Y + LineSize/2,
		Center.X, Center.Y + LineSize*2,
		LineColor, LineWidth);
	
	DrawLine(Center.X, Center.Y - LineSize/2,
		Center.X , Center.Y - LineSize*2,
		LineColor, LineWidth);
}
