// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTService_CheckRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CC_EnemyCharacter.h"
#include "Characters/CC_PlayerCharacter.h"

void UCC_BTService_CheckRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	UBlackboardComponent* BB = AIController->GetBlackboardComponent();
	if (!IsValid(BB)) return;

	ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(BB->GetValueAsObject("TargetActor"));
	ACC_EnemyCharacter* EnemyCharacter = Cast<ACC_EnemyCharacter>(AIController->GetPawn());
	if (IsValid(PlayerCharacter) && PlayerCharacter->IsAlive() && IsValid(EnemyCharacter) && EnemyCharacter->IsAlive())
	{
		float Distance = FVector::Distance(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
		UE_LOG(LogTemp, Warning, TEXT("AcceptanceRadius entered if! %f %f %f"), EnemyCharacter->AcceptanceRadius, BB->GetValueAsFloat("AcceptanceRadius"), Distance);
		BB->SetValueAsBool("bIsTargetInRange", Distance <= EnemyCharacter->AcceptanceRadius);
		BB->SetValueAsBool("bIsTargetTooClose", EnemyCharacter->bIsMelee ? false : Distance < EnemyCharacter->AcceptanceRadius - 400);
	}
	else
	{
		BB->SetValueAsBool("bIsTargetInRange", false);
		BB->SetValueAsBool("bIsTargetTooClose", false);
	}
}