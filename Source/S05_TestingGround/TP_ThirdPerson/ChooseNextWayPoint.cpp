// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWayPoint.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "PatrolRouteComponent.h" 

EBTNodeResult::Type UChooseNextWayPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	// Get Patrol Points 
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRoute = ControlledPawn->FindComponentByClass<UPatrolRouteComponent>();
	if (!ensure(PatrolRoute)) { return EBTNodeResult::Failed; }
	// warn against empty patrol routes
	auto PatrolPoints = PatrolRoute->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A gaurd is missing patrol points:"));
		return EBTNodeResult::Failed;
	}
	// Set Next WayPoint
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WayPointKey.SelectedKeyName, PatrolPoints[Index]);

	// Cycle the index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);
	return EBTNodeResult::Succeeded;

}