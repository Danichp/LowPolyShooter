// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_Pause.h"

void UTMS_Pause::NativeConstruct()
{
	Super::NativeConstruct();

	check(VB_Buttons);
	check(B_Resume);
	check(B_ChangeLocale);
	check(B_Exit);
}
