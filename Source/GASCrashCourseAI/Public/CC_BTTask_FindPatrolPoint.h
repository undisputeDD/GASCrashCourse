// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CC_BTTask_FindPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_BTTask_FindPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCC_BTTask_FindPatrolPoint();

	UPROPERTY(EditAnywhere, Category = "Crash|AI|Blackboard")
	FBlackboardKeySelector PatrolPointKey;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
