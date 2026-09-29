// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/MenuGM.h"
#include "PlayerController/MenuPC.h"
#include "HUD/MenuHUD.h"

AMenuGM::AMenuGM() : Super()
{
	// use our custom C++ Menu PlayerController class
	PlayerControllerClass = AMenuPC::StaticClass();

	// use our custom C++ Menu HUD class
	HUDClass = AMenuHUD::StaticClass();
}
