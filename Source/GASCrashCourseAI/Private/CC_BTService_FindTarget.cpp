// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTService_FindTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Utils/CC_BlueprintLibrary.h"
#include "Characters/CC_BaseCharacter.h"

void UCC_BTService_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	FClosestActorWithTagResult ClosestActorResult = UCC_BlueprintLibrary::FindClosestActorWithTag(
		ControlledPawn,
		ControlledPawn->GetActorLocation(),
		CrashTags::Player,
		SearchRange
	);

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	ACC_BaseCharacter* TargetPlayerActor = Cast<ACC_BaseCharacter>(ClosestActorResult.Actor);

	if (IsValid(TargetPlayerActor) && TargetPlayerActor->IsAlive())
	{
		Blackboard->SetValueAsObject(FName("TargetActor"), TargetPlayerActor);
	}
	else
	{
		Blackboard->ClearValue(FName("TargetActor"));
	}

	// DrawDebugSphere(ControlledPawn->GetWorld(), ControlledPawn->GetActorLocation(), SearchRange, 16, FColor::Blue, false, 3.f);
}
