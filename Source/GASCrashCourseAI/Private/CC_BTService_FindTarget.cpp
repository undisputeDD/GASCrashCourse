// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTService_FindTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Utils/CC_BlueprintLibrary.h"
#include "Characters/CC_BaseCharacter.h"
#include "Characters/CC_EnemyCharacter.h"

void UCC_BTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	ACC_EnemyCharacter* EnemyCharacter = Cast<ACC_EnemyCharacter>(AIController->GetPawn());
	if (!IsValid(EnemyCharacter) || !EnemyCharacter->IsAlive())
	{
		if (Blackboard)
		{
			Blackboard->ClearValue(FName("TargetActor"));
			Blackboard->SetValueAsBool(FName("bIsTargetInMeleeRange"), false);
		}
		return;
	}

	FClosestActorWithTagResult ClosestActorResult = UCC_BlueprintLibrary::FindClosestActorWithTag(
		EnemyCharacter,
		EnemyCharacter->GetActorLocation(),
		CrashTags::Player,
		EnemyCharacter->SearchRange
	);

	ACC_BaseCharacter* TargetPlayerActor = Cast<ACC_BaseCharacter>(ClosestActorResult.Actor);

	if (IsValid(TargetPlayerActor) && TargetPlayerActor->IsAlive() && IsValid(Blackboard))
	{
		Blackboard->SetValueAsObject(FName("TargetActor"), TargetPlayerActor);
		float DistanceToPlayer = FVector::Distance(EnemyCharacter->GetActorLocation(), TargetPlayerActor->GetActorLocation());
		bool bInRange = (DistanceToPlayer <= EnemyCharacter->AcceptanceRadius);
		Blackboard->SetValueAsBool(FName("bIsTargetInMeleeRange"), bInRange);
	}
	else
	{
		if (IsValid(Blackboard))
		{
			Blackboard->ClearValue(FName("TargetActor"));
			Blackboard->SetValueAsBool(FName("bIsTargetInMeleeRange"), false);
		}
	}

	// DrawDebugSphere(EnemyCharacter->GetWorld(), EnemyCharacter->GetActorLocation(), EnemyCharacter->SearchRange, 16, FColor::Blue, false, 3.f);
}
