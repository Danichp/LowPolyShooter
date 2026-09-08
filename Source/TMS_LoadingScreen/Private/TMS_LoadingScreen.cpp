#include "TMS_LoadingScreen.h"

#include "MoviePlayer.h"
#include "TMS_LoadingScreenSettings.h"
#include "LoadingScreen.h"

#define LOCTEXT_NAMESPACE "FTMS_LoadingScreenModule"

DEFINE_LOG_CATEGORY(LogTMS_LoadingScreen);

void FTMS_LoadingScreenModule::StartupModule()
{
    UE_LOG(LogTMS_LoadingScreen, Display, TEXT("FTMS_LoadingScreenModule::StartupModule()"));

    const UTMS_LoadingScreenSettings* TMSLSS = UTMS_LoadingScreenSettings::Get();
    
    if (TMSLSS && TMSLSS->BGTexture.IsValid())
    {
        LoadingScreenBackground = LoadObject<UTexture2D>(nullptr, *TMSLSS->BGTexture.ToString());
    }
    else
    {
        LoadingScreenBackground = LoadObject<UTexture2D>(nullptr, TEXT("/Game/TMS_LS/Textures/T_Background.T_Background"));
    }
}

void FTMS_LoadingScreenModule::ShutdownModule()
{
    LoadingScreenBackground = nullptr;
}

void FTMS_LoadingScreenModule::StartLoadingScreen(const FString& MapName)
{
    UE_LOG(LogTMS_LoadingScreen, Log, TEXT("FTMS_LoadingScreenModule::StartLoadingScreen"));
    
    FLoadingScreenAttributes LS;
    LS.bAutoCompleteWhenLoadingCompletes = false;
    LS.bMoviesAreSkippable = true;
    LS.MoviePaths.Add(TEXT("LoadingScreenMovies"));
    LS.MinimumLoadingScreenDisplayTime = UTMS_LoadingScreenSettings::Get()->MinimumDisplayTime;
    LS.WidgetLoadingScreen = SNew(SLoadingScreen)
        .BGTexture(LoadingScreenBackground.Get())
        .MapName(FText::FromString(MapName));//FLoadingScreenAttributes::NewTestLoadingScreenWidget();
    
    GetMoviePlayer()->SetupLoadingScreen(LS);
    
    GetMoviePlayer()->PlayMovie();
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FTMS_LoadingScreenModule, TMS_LoadingScreen)