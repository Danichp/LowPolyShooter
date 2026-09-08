// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class TMS_LOADINGSCREEN_API SLoadingScreen : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLoadingScreen)
		: _BGTexture(nullptr)
		{}
		SLATE_ARGUMENT(UTexture2D*, BGTexture)
		SLATE_ARGUMENT(FText, MapName)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
private:
	UTexture2D* BGTexture;
	FText MapName;
	
	TSharedPtr<FSlateBrush> BGBrush;
	TSharedPtr<FSlateFontInfo> FontData;
};
