// Fill out your copyright notice in the Description page of Project Settings.
#include "EnemyController.h"
#include "NavigationSystem.h"
#include "EnemyCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "MainCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISenseConfig_Hearing.h"
AEnemyController::AEnemyController()
	: Super()
{
	// Initialize perception component and senses
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	// Create the senses
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	// Configure sight sense
	SightConfig->SightRadius = 600.f;
	SightConfig->LoseSightRadius = 700.f;
	SightConfig->PeripheralVisionAngleDegrees = 45.f;
	SightConfig->SetMaxAge(5.f);
	SightConfig->AutoSuccessRangeFromLastSeenLocation = 900.f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Configure hearing sense
	HearingConfig->HearingRange = 1500.f;
	HearingConfig->SetMaxAge(2);
	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Add senses to perception component
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->ConfigureSense(*HearingConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}
void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	// Bind perception update event
	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyController::OnTargetPerceptionUpdated);
}
void AEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (InPawn == nullptr)
	{
		return;
	}
	if (AEnemyCharacter* Enemy = Cast<AEnemyCharacter>(InPawn))
	{
		if (Enemy->BehaviorTree)
		{
			RunBehaviorTree(Enemy->BehaviorTree);
		}
	}
}
void AEnemyController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bCanSeePlayer)
	{
		GEngine->AddOnScreenDebugMessage(1, 0.1f, FColor::Red, TEXT("I SEE YOU!"));
	}
	else if (bSoundHeard)
	{
		GEngine->AddOnScreenDebugMessage(2, 0.1f, FColor::Orange, TEXT("STOP SHOOTING!"));
	}
}
void AEnemyController::StopBehaviorTree()
{
	//BrainComponent->StopLogic(TEXT("Player is dead, Behavior Tree has stopped"));
	BrainComponent->Cleanup();
}
void AEnemyController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// Check if the actor is valid
	if (Actor == nullptr) return;
	// Check if the actor is a Main Character
	if (AMainCharacter* SensedCharacter = Cast<AMainCharacter>(Actor))
	{
		// Check if the stimulus is sight
		if (Stimulus.Type == SightConfig->GetSenseID())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				bCanSeePlayer = true;
				// bSoundHeard = false;
			}
			else
			{
				bCanSeePlayer = false;
				// bSoundHeard = true;
			}
		}
		// Check if the stimulus is hearing
		else if (Stimulus.Type == HearingConfig->GetSenseID())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				bSoundHeard = true;
				// bCanSeePlayer = false;

				// Clear bSoundHeard after 2 seconds so the message stops looping
				// Hearing is a one-shot event with no "lost hearing" callback unlike sight
				FTimerHandle HearingTimer;
				GetWorldTimerManager().SetTimer(HearingTimer, [this]()
					{
						bSoundHeard = false;
					}, 2.0f, false);
			}
			else
			{
				bSoundHeard = false;
				// bCanSeePlayer = true;
			}
		}
	}
	else
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Orange, TEXT("I SEE SOME AMMO!"));
		}
	}
}