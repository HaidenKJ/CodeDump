// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/BaseGM.h"
#include "PlayerController/BasePC.h"
#include "HUD/BaseHUD.h"

ABaseGM::ABaseGM() : Super()
{
	// use our custom C++ Base PlayerController class
	PlayerControllerClass = ABasePC::StaticClass();

	// use our custom C++ Base HUD class
	HUDClass = ABaseHUD::StaticClass();
}

