// Fill out your copyright notice in the Description page of Project Settings.


#include "TMS_GMTest.h"

#include "TMS_HUD.h"
#include "TMS_Player.h"
#include "TMS_PlayerController.h"

ATMS_GMTest::ATMS_GMTest()
{
	DefaultPawnClass = ATMS_Player::StaticClass();
	HUDClass = ATMS_HUD::StaticClass();
	PlayerControllerClass = ATMS_PlayerController::StaticClass();
}
