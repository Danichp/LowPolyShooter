// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_LoadingScreenSettings.h"

UTMS_LoadingScreenSettings::UTMS_LoadingScreenSettings(const FObjectInitializer& initializer)
{
	
}

const UTMS_LoadingScreenSettings* UTMS_LoadingScreenSettings::Get()
{
	return GetDefault<UTMS_LoadingScreenSettings>();
}
