// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameGM.h"
#include "PlayerController/GamePC.h"
#include "HUD/GameHUD.h"
#include "CombProj_HKCharacter.h"

AGameGM::AGameGM() : Super()
{
	// use our custom C++ Game PlayerController class
	PlayerControllerClass = AGamePC::StaticClass();

	// use our custom C++ Game HUD class
	HUDClass = AGameHUD::StaticClass();

	// use our C++ Basic Character class
	DefaultPawnClass = ACombProj_HKCharacter::StaticClass();
}

