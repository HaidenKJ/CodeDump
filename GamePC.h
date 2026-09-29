// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerController/BasePC.h"
#include "GamePC.generated.h"

class UInputMappingContext;

/**
 * 
 */
UCLASS()
class COMBPROJ_HK_API AGamePC : public ABasePC
{
	GENERATED_BODY()
	
	/** Constructor */
	AGamePC();

protected:

	/** Input Mapping Contexts */
	UPROPERTY(EditAnywhere, Category = "Input|Input Mappings")
	TArray<UInputMappingContext*> DefaultMappingContexts;

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Input mapping context setup */
	virtual void SetupInputComponent() override;

};
