// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StreamableManager.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/Notifications/SProgressBar.h"

/**
 * 
 */

class FTMSHandleWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( FTMSHandleWidget ){}
		SLATE_ARGUMENT( TSharedPtr<FStreamableHandle>, Handle )
	SLATE_END_ARGS()
	
	void Construct(const FArguments& InArgs)
	{
		StreamingHandle = InArgs._Handle;
	}
	
	 FTMSHandleWidget& operator= ( FTMSHandleWidget& InArgs)
	{
		return InArgs;
	}
	
	TSharedPtr<FStreamableHandle> StreamingHandle;
};


class TMS_LS_API STMS_LoadingScreenV1 : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(STMS_LoadingScreenV1) {}
	SLATE_ARGUMENT(FName, LevelName)
	SLATE_ARGUMENT(float, Progress)
	SLATE_ARGUMENT(TSharedPtr<FTMSHandleWidget>, HandleWidget)
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, 
		const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements,
		int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	
	
protected:
	
	float GetDPIScale() const;
	
private:
	
	TSharedPtr<STextBlock> LoadingTextBlock;
	TSharedPtr<SImage> LoadingIcon;
	TSharedPtr<SProgressBar> LoadingProgressBar;
	
	TSharedPtr<FTMSHandleWidget> HandleWidget;
	
	mutable float AlphaFade = 0.0f;
	mutable float TotalDeltaTime = 0.0f;
	float FadeTimeRemaining = 0.05f;
	
	TSharedPtr<SBorder> LoadingFade;
	
	float CurProgress = 0.0f;
	FName LoadingText;
};
