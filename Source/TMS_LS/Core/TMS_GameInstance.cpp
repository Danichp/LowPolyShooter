// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_GameInstance.h"

#include "TMS_LoadingScreen.h"
#include "Blueprint/UserWidget.h"
#include "Engine/AssetManager.h"
#include "Kismet/GameplayStatics.h"
#include "TMS_LS/UI/Loading/TMS_LoadingScreenV1.h"

void UTMS_GameInstance::BeginLoadLevel(const FString& String)
{
	FTMS_LoadingScreenModule* LSM = FModuleManager::LoadModulePtr<FTMS_LoadingScreenModule>("TMS_LoadingScreen");
	if (LSM)
	{
		LSM->StartLoadingScreen(String);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("UTMS_GameInstance::BeginLoadLevel - Loading screen module is not found"));
	}
}

void UTMS_GameInstance::EndLoadLevel(UWorld* World)
{
}

void UTMS_GameInstance::Init()
{
	Super::Init();
	// if (!LoadingWidgetClass)
	// {
	// 	ConstructorHelpers::FClassFinder<UUserWidget> DefaultLoadingWidgetClass(
	// 		TEXT("/Game/TMS_LS/UI/WBP_LoadingScreen.WBP_LoadingScreen"));
	// 	if (DefaultLoadingWidgetClass.Succeeded())
	// 	{
	// 		LoadingWidgetClass = DefaultLoadingWidgetClass.Class;
	// 	}
	// }
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UTMS_GameInstance::BeginLoadLevel);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UTMS_GameInstance::EndLoadLevel);
}

void UTMS_GameInstance::Shutdown()
{
	//HideLoadingScreen();
	Super::Shutdown();
}

void UTMS_GameInstance::LoadLevel(const FName& LevelName)
{
	PendingLevelName = LevelName;
	ShowLoadingScreen();
	
	GetWorld()->GetTimerManager().SetTimer(TravelTimerHandle, [this]()
	{
		UGameplayStatics::OpenLevel(GetWorld(), PendingLevelName);
		PendingLevelName = NAME_None;
	}, 1.f, false);
}

void UTMS_GameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
	HideLoadingScreen();
}

void UTMS_GameInstance::ShowLoadingScreen()
{
	if (!LoadingWidgetClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("Loading screen is not available"));
		return;
	}
	
	if (GetGameViewportClient())
	{
		TSharedPtr<FTMSHandleWidget> HandleWidget = SNew(FTMSHandleWidget).Handle(CurrentLoadingHandle);
		LoadingWidgetInst = SNew(STMS_LoadingScreenV1).LevelName(PendingLevelName).HandleWidget(HandleWidget);
		if (LoadingWidgetInst.IsValid())
		{
			GetGameViewportClient()->AddViewportWidgetContent(LoadingWidgetInst.ToSharedRef(), 1000);
		}
	}
	
	// HideLoadingScreen();
	//
	// if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
	// {
	// 	LoadingWidgetInstance = CreateWidget<UUserWidget>(PC, LoadingWidgetClass);
	// 	if (LoadingWidgetInstance)
	// 	{
	// 		LoadingWidgetInstance->AddToViewport(100);
	// 		UpdateLoadingText();
	// 	}
	// }
}

void UTMS_GameInstance::HideLoadingScreen()
{
	// if (LoadingWidgetInstance)
	// {
	// 	LoadingWidgetInstance->RemoveFromParent();
	// 	LoadingWidgetInstance = nullptr;
	// }
	
	if (GetGameViewportClient() && LoadingWidgetInst.IsValid())
	{
		GetGameViewportClient()->RemoveViewportWidgetContent(LoadingWidgetInst.ToSharedRef());
		LoadingWidgetInst.Reset();
	}
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(TravelTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(AsyncLoadTimerHandle);
	}
	
	CurrentLoadingHandle.Reset();
	CurrentLoadingProgress = 0.f;
}

void UTMS_GameInstance::UpdateLoadingProcess(float Progress) const
{
	if (LoadingWidgetInstance)
	{
		LoadingWidgetInstance->AddToViewport(100);

		if (UFunction* SetProgressFunc = LoadingWidgetInstance->FindFunction(FName("SetProgressFunc")))
		{
			struct FSetTextParams
			{
				float ProgressParam;
			} Params;
			Params.ProgressParam = Progress;
			LoadingWidgetInstance->ProcessEvent(SetProgressFunc, &Params);
		}
	}
}

void UTMS_GameInstance::UpdateLoadingText(const FText& InText) const
{
	if (LoadingWidgetInstance)
	{

		if (UFunction* SetTextFunc = LoadingWidgetInstance->FindFunction(FName("SetTextFunc")))
		{
			struct FSetTextParams
			{
				FText TextParam;
			} Params;
			
			Params.TextParam = InText.IsEmpty() ? 
				FText::FromName(PendingLevelName) :	InText;
			
			LoadingWidgetInstance->ProcessEvent(SetTextFunc, &Params);
		}
	}
}

void UTMS_GameInstance::AsyncLoadLevel(const FName& LevelName)
{
	PendingLevelName = LevelName;
	PerformAsyncLoad();
	ShowLoadingScreen();
}

void UTMS_GameInstance::PerformAsyncLoad()
{
	if (PendingLevelName.IsNone())
	{
		return;
	}
	
	FString LevelPath = FString::Printf(TEXT("/Game/TMS_LS/Levels/%s"), *PendingLevelName.ToString());
	
	CurrentLoadingHandle = UAssetManager::GetStreamableManager().RequestAsyncLoad(
		FSoftObjectPath(LevelPath), 
		FStreamableDelegate::CreateUObject(this, &UTMS_GameInstance::CheckProgress)
		);
	
	GetWorld()->GetTimerManager().SetTimer(AsyncLoadTimerHandle, this,
		&UTMS_GameInstance::CheckProgress, 0.1f, true);
}

void UTMS_GameInstance::CheckProgress()
{
	if (CurrentLoadingHandle.IsValid() && CurrentLoadingHandle->HasLoadCompleted())
	{
		GetWorld()->GetTimerManager().ClearTimer(AsyncLoadTimerHandle);
		UpdateLoadingProcess(1.f);
		
		UGameplayStatics::OpenLevel(GetWorld(), PendingLevelName);
		PendingLevelName = NAME_None;
		CurrentLoadingHandle.Reset();
		//HideLoadingScreen();
	}
	else if (CurrentLoadingHandle.IsValid())
	{
		CurrentLoadingProgress = CurrentLoadingHandle->GetProgress();
		UpdateLoadingProcess(CurrentLoadingProgress);
		UpdateLoadingText(FText::FromString(
			FString::Printf(TEXT("Loading... %.0f%%"), CurrentLoadingProgress * 100.f)));
	}
}
