// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CC_BTTask_SmoothRotateToTarget.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_BTTask_SmoothRotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCC_BTTask_SmoothRotateToTarget();

	UPROPERTY(EditAnywhere, Category = "Crash|AI|Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category = "Crash|AI|Settings", meta = (ClampMin = "0.0", UIMin = "0.0"))
	float RotationSpeed = 180.0f;

	UPROPERTY(EditAnywhere, Category = "Crash|AI|Settings", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float AngleTolerance = 2.0f;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
