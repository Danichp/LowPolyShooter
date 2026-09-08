// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_DeveloperSettings.h"

UTMS_DeveloperSettings::UTMS_DeveloperSettings(const FObjectInitializer& ObjectInitializer)
{
}

const UTMS_DeveloperSettings* UTMS_DeveloperSettings::Get()
{
    return GetDefault<UTMS_DeveloperSettings>();
}