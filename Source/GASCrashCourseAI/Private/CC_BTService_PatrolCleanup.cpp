// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTService_PatrolCleanup.h"
#include "CC_AI_PatrolWorldSubsystem.h"
#include "CC_AI_PatrolPoint.h"
#include "BehaviorTree/BlackboardComponent.h"

UCC_BTService_PatrolCleanup::UCC_BTService_PatrolCleanup()
{
	NodeName = "Patrol Cleanup";
	bNotifyCeaseRelevant = true;
}

void UCC_BTService_PatrolCleanup::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	UWorld* World = OwnerComp.GetWorld();

	if (!BB || !World) return;

	ACC_AI_PatrolPoint* Point = Cast<ACC_AI_PatrolPoint>(BB->GetValueAsObject(PatrolPointKey.SelectedKeyName));

	if (Point)
	{
		if (UCC_AI_PatrolWorldSubsystem* PatrolSubsystem = World->GetSubsystem<UCC_AI_PatrolWorldSubsystem>())
		{
			PatrolSubsystem->ReleasePoint(Point);
		}

		BB->ClearValue(PatrolPointKey.SelectedKeyName);
	}
}