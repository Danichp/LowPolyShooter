#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTMS_LoadingScreen, Log, All);

class FTMS_LoadingScreenModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
    
    virtual bool IsGameModule() const override { return true; }
    
    virtual void StartLoadingScreen(const FString& MapName);
    
private:
    TObjectPtr<UTexture2D> LoadingScreenBackground;
};
