// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTTask_SmoothRotateToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCC_BTTask_SmoothRotateToTarget::UCC_BTTask_SmoothRotateToTarget()
{
	NodeName = TEXT("Smooth Rotate To Target");
	bNotifyTick = true;
}

EBTNodeResult::Type UCC_BTTask_SmoothRotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!IsValid(AIC) || !IsValid(AIC->GetPawn())) return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}

void UCC_BTTask_SmoothRotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* AIC = OwnerComp.GetAIOwner();
	if (!IsValid(AIC)) return;

	APawn* ControlledPawn = AIC->GetPawn();
	if (!IsValid(ControlledPawn)) return;

	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	if (!IsValid(BB)) return;

	AActor* TargetActor = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));

	if (!IsValid(TargetActor))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	FVector StartLocation = ControlledPawn->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(StartLocation, TargetLocation);

	FRotator CurrentRotation = ControlledPawn->GetActorRotation();
	TargetRotation.Pitch = CurrentRotation.Pitch;
	TargetRotation.Roll = CurrentRotation.Roll;

	FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);

	ControlledPawn->SetActorRotation(NewRotation);

	float AngleDifference = FMath::Abs(NewRotation.Yaw - TargetRotation.Yaw);

	if (AngleDifference > 180.0f) { AngleDifference = 360.0f - AngleDifference; }

	if (AngleDifference <= AngleTolerance)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}