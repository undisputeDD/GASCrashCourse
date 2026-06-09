// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CC_EnemyAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API ACC_EnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACC_EnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crash|AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	TObjectPtr<UBlackboardComponent> BlackboardComponent;
	
};
