// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_LoadingScreenV1.h"

#include "SlateOptMacros.h"
#include "Engine/AssetManager.h"
#include "Engine/UserInterfaceSettings.h"
#include "Widgets/Layout/SDPIScaler.h"
#include "Widgets/Layout/SScaleBox.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STMS_LoadingScreenV1::Construct(const FArguments& InArgs)
{
	LoadingText = InArgs._LevelName;
	HandleWidget = InArgs._HandleWidget;
	
	FFormatNamedArguments Args;
	Args.Add("LoadingText", FText::FromString(LoadingText.ToString()));
	
	ChildSlot
	[
		SNew(SDPIScaler)
		.DPIScale(this, &STMS_LoadingScreenV1::GetDPIScale)
		[
			SNew(SBorder)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
			.BorderBackgroundColor(FColor(0.05f, 0.05f, 0.05f, 1.f))
			[
				SNew(SOverlay)
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
						SNew(SScaleBox)
					.Stretch(EStretch::ScaleToFit)
					.StretchDirection(EStretchDirection::Both)
					[
						SNew(SBox)
						.WidthOverride(1920.f)
						.HeightOverride(1080.f)
						[
							SNew(SOverlay)
							+ SOverlay::Slot()
	                        .HAlign(HAlign_Left)
	                        .VAlign(VAlign_Bottom)
	                        .Padding(50.f, 0.f, 0.f, -50.f)
	                        [
                        		SAssignNew(LoadingTextBlock, STextBlock)
                        		.Text(FText::FromString(LoadingText.ToString()))
	                        ]
	                        + SOverlay::Slot()
	                        .HAlign(HAlign_Fill)
	                        .VAlign(VAlign_Bottom)
	                        [
                        		SNew(SBox)
                        		.HeightOverride(8.f)
                        		[
                        			SAssignNew(LoadingProgressBar, SProgressBar)
                        			.FillColorAndOpacity(FLinearColor::Yellow)
                        		]
	                        ]
						]
					]
				]
				+SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SAssignNew(LoadingFade, SBorder)
					.HAlign(HAlign_Fill)
					.VAlign(VAlign_Fill)
					.BorderImage(FCoreStyle::Get().GetBrush("WhiteBrush"))
					.BorderBackgroundColor(FColor::Black)
				]
			]
		]
	];
	
	AlphaFade = 1.f;
	FadeTimeRemaining = 2.f;
	SetColorAndOpacity(FLinearColor::White);
}

int32 STMS_LoadingScreenV1::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	float LIncrementFade = (1.f/FadeTimeRemaining) * Args.GetDeltaTime();
	if (AlphaFade > 0.f && FadeTimeRemaining != 0.f)
	{
		AlphaFade = FMath::Clamp(AlphaFade-LIncrementFade, 0.f, 1.f);
		LoadingFade->SetRenderOpacity(AlphaFade);
	}
	
	if (HandleWidget.IsValid() && 
		HandleWidget->StreamingHandle.IsValid() && 
		HandleWidget->StreamingHandle->IsLoadingInProgress())
	{
		LoadingProgressBar->SetPercent(HandleWidget->StreamingHandle->GetProgress());
	}
	
	return SCompoundWidget::OnPaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                                bParentEnabled);
}


float STMS_LoadingScreenV1::GetDPIScale() const
{
	const FVector2D& DrawSize = GetTickSpaceGeometry().ToPaintGeometry().GetLocalSize();
	const FIntPoint Size((int32) DrawSize.X, (int32) DrawSize.Y);
	return GetDefault<UUserInterfaceSettings>()->GetDPIScaleBasedOnSize(Size);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
