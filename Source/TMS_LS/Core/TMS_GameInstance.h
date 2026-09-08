// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "TMS_GameInstance.generated.h"

class STMS_LoadingScreenV1;
/**
 * 
 */
UCLASS()
class TMS_LS_API UTMS_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	void BeginLoadLevel(const FString& String);
	void EndLoadLevel(UWorld* World);
	
	virtual void Init() override;
	virtual void Shutdown() override;
	
	UFUNCTION(BlueprintCallable, Category = "Loading")
	void LoadLevel(const FName& LevelName);
	
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
	
	UFUNCTION(BlueprintCallable, Category = "Loading")
	void AsyncLoadLevel(const FName& LevelName);
	
protected:
	void ShowLoadingScreen();
	void HideLoadingScreen();
	void UpdateLoadingProcess(float Progress) const;
	void UpdateLoadingText(const FText& InText = FText::GetEmpty()) const;
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loading")
	TSubclassOf<UUserWidget> LoadingWidgetClass;
	
private:
	UPROPERTY()
	UUserWidget* LoadingWidgetInstance;
	
	TSharedPtr<STMS_LoadingScreenV1> LoadingWidgetInst;
	
	FName PendingLevelName;
	FTimerHandle TravelTimerHandle;
	FTimerHandle AsyncLoadTimerHandle;
	
	void PerformAsyncLoad();
	void CheckProgress();
	TSharedPtr<FStreamableHandle> CurrentLoadingHandle;
	float CurrentLoadingProgress;
};
