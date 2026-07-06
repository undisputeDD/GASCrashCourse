// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_AI_PatrolWorldSubsystem.h"
#include "CC_AI_PatrolPoint.h"
#include "Kismet/GameplayStatics.h"

void UCC_AI_PatrolWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	CachedPatrolPoints.Empty();

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(&InWorld, ACC_AI_PatrolPoint::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		if (ACC_AI_PatrolPoint* Point = Cast<ACC_AI_PatrolPoint>(Actor))
		{
			CachedPatrolPoints.Add(Point);
		}
	}
}

ACC_AI_PatrolPoint* UCC_AI_PatrolWorldSubsystem::GetAndClaimRandomFreePoint()
{
	TArray<ACC_AI_PatrolPoint*> FreePoints;

	for (ACC_AI_PatrolPoint* Point : CachedPatrolPoints)
	{
		if (IsValid(Point) && !Point->bIsOccupied)
		{
			FreePoints.Add(Point);
		}
	}

	if (FreePoints.IsEmpty()) return nullptr;

	int32 RandomIndex = FMath::RandRange(0, FreePoints.Num() - 1);
	ACC_AI_PatrolPoint* ChosenPoint = FreePoints[RandomIndex];

	ChosenPoint->bIsOccupied = true;
	return ChosenPoint;
}

void UCC_AI_PatrolWorldSubsystem::ReleasePoint(ACC_AI_PatrolPoint* Point)
{
	if (IsValid(Point))
	{
		Point->bIsOccupied = false;
	}
}