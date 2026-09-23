// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskNode_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "AIController.h"

EBTNodeResult::Type UBTTaskNode_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (AIController == nullptr) { return EBTNodeResult::Failed;  }

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (BlackboardComp == nullptr) { return EBTNodeResult::Failed; }

	APawn* AIPawn = AIController->GetPawn();
	if (AIPawn == nullptr) { return EBTNodeResult::Failed; }

	UWorld* World = AIPawn->GetWorld();
	if (World == nullptr) { return EBTNodeResult::Failed; }

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(World);
	if (NavSystem == nullptr) { return EBTNodeResult::Failed; }

	// Get the origin]
	FVector Origin = AIPawn->GetActorLocation();

	// Get the random location
	FNavLocation RandomLocation;
	if (NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, RandomLocation))
	{
		BlackboardComp->SetValueAsVector(DestinationVectorKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
