// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTTask_FindPatrolPoint.h"
#include "CC_AI_PatrolWorldSubsystem.h"
#include "CC_AI_PatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

UCC_BTTask_FindPatrolPoint::UCC_BTTask_FindPatrolPoint()
{
	NodeName = "Find Free Patrol Point";
}

EBTNodeResult::Type UCC_BTTask_FindPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UWorld* World = OwnerComp.GetWorld();
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();

	if (!World || !BB) return EBTNodeResult::Failed;

	UCC_AI_PatrolWorldSubsystem* PatrolSubsystem = World->GetSubsystem<UCC_AI_PatrolWorldSubsystem>();
	if (!PatrolSubsystem) return EBTNodeResult::Failed;

	ACC_AI_PatrolPoint* FreePoint = PatrolSubsystem->GetAndClaimRandomFreePoint();

	if (FreePoint)
	{
		BB->SetValueAsObject(PatrolPointKey.SelectedKeyName, FreePoint);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}