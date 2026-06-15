// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CC_AI_PatrolWorldSubsystem.generated.h"

class ACC_AI_PatrolPoint;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_AI_PatrolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	ACC_AI_PatrolPoint* GetAndClaimRandomFreePoint();

	void ReleasePoint(ACC_AI_PatrolPoint* Point);

private:
	UPROPERTY()
	TArray<ACC_AI_PatrolPoint*> CachedPatrolPoints;
};
