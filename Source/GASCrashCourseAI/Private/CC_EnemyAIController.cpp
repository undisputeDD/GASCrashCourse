// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CC_EnemyCharacter.h"

ACC_EnemyAIController::ACC_EnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ACC_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset != nullptr)
	{
		if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(InPawn))
		{
			BlackboardComponent->SetValueAsFloat(FName("AcceptanceRadius"), Enemy->AcceptanceRadius);
		}

		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}
}
