// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyController.generated.h"

UCLASS()
class LAB1_KAYMENJ_API AEnemyController : public AAIController
{
	GENERATED_BODY()

public:

	AEnemyController();

	virtual void OnPossess(APawn* InPawn) override;

	void StopBehaviorTree();

public: 
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float Deltaseconds) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "AI")
	bool bCanSeePlayer;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	bool bSoundHeard;
	
protected:
	//UPROPERTY(VisibleAnywhere, Category = "AI")
	//TObjectPtr<class UAIPerceptionComponent> AIPerceptionComponent;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<class UAISenseConfig_Hearing> HearingConfig;
};