// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CC_BTService_PatrolCleanup.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_BTService_PatrolCleanup : public UBTService
{
	GENERATED_BODY()
	
public:
	UCC_BTService_PatrolCleanup();

	UPROPERTY(EditAnywhere, Category = "Crash|AI|Blackboard")
	FBlackboardKeySelector PatrolPointKey;

protected:
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
