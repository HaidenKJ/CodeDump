// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/GamePC.h"

// Copyright Epic Games, Inc. All Rights Reserved.

#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "CombProj_HKCameraManager.h"
#include "CombProj_HK.h"

AGamePC::AGamePC()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ACombProj_HKCameraManager::StaticClass();
}

void AGamePC::BeginPlay()
{
	Super::BeginPlay();
}

void AGamePC::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}
		}
	}
}
