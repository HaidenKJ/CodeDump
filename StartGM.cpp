// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/StartGM.h"
#include "PlayerController/StartPC.h"
#include "HUD/StartHUD.h"

AStartGM::AStartGM() : Super()
{
	// use our custom C++ Start PlayerController class
	PlayerControllerClass = AStartPC::StaticClass();

	// use our custom C++ Start HUD class
	HUDClass = AStartHUD::StaticClass();
}

