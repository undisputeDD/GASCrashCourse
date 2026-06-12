// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTTask_SetAttackDelay.h"
#include "Characters/CC_EnemyCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCC_BTTask_SetAttackDelay::UCC_BTTask_SetAttackDelay()
{
	NodeName = "Set Enemy Attack Delay to BB";
}

EBTNodeResult::Type UCC_BTTask_SetAttackDelay::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return EBTNodeResult::Failed;

	ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(AIController->GetPawn());
	if (!IsValid(Enemy)) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!IsValid(Blackboard)) return EBTNodeResult::Failed;

	const float RandomDelay = FMath::RandRange(Enemy->MinAttackDelay, Enemy->MaxAttackDelay);

	Blackboard->SetValueAsFloat(DelayKey.SelectedKeyName, RandomDelay);

	return EBTNodeResult::Succeeded;
}
