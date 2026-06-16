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
		UE_LOG(LogTemp, Warning, TEXT("AcceptanceRadius entered if!"));
		float Distance = FVector::Distance(EnemyCharacter->GetActorLocation(), PlayerCharacter->GetActorLocation());
		BB->SetValueAsBool("bIsTargetInRange", Distance <= EnemyCharacter->AcceptanceRadius);
	}
	else
	{
		BB->SetValueAsBool("bIsTargetInRange", false);
	}
}