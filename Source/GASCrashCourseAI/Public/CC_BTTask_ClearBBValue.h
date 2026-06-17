// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CC_BTTask_ClearBBValue.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_BTTask_ClearBBValue : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCC_BTTask_ClearBBValue();

	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector KeyToClear;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
