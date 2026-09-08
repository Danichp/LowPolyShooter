// Fill out your copyright notice in the Description page of Project Settings.


#include "LoadingScreen.h"

#include "SlateOptMacros.h"
#include "Widgets/Images/SThrobber.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void SLoadingScreen::Construct(const FArguments& InArgs)
{
	BGTexture = InArgs._BGTexture;
	MapName = InArgs._MapName;
	
	BGBrush = MakeShareable(new FSlateBrush());
	BGBrush->SetResourceObject(BGTexture);
	
	FontData = MakeShareable(new FSlateFontInfo());
	FontData->Size = 24.f;
	FontData->OutlineSettings.OutlineSize = 2.f;
	FontData->OutlineSettings.OutlineColor = FLinearColor::Black;
	
	ChildSlot
	[
		SNew(SOverlay)
		+SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SImage)
			.Image(BGTexture ? BGBrush.Get() : nullptr)
			.ColorAndOpacity(FLinearColor::White)
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(15.f)
		[
			SNew(SThrobber)
			.Visibility(EVisibility::HitTestInvisible)
			.NumPieces(10)
		]
		+SOverlay::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Top)
		.Padding(15.f)
		[
			SNew(STextBlock)
			.Text(MapName)
			.ColorAndOpacity(FLinearColor::White)
			.Font(*FontData.Get())
		]
	];
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
