// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTTask_ClearBBValue.h"
#include "BehaviorTree/BlackboardComponent.h"

UCC_BTTask_ClearBBValue::UCC_BTTask_ClearBBValue()
{
	NodeName = "Clear Blackboard Value";
}

EBTNodeResult::Type UCC_BTTask_ClearBBValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BB)) return EBTNodeResult::Failed;

	BB->ClearValue(KeyToClear.SelectedKeyName);

	return EBTNodeResult::Succeeded;
}