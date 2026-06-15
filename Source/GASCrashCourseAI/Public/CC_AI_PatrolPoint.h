// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CC_AI_PatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API ACC_AI_PatrolPoint : public AActor
{
	GENERATED_BODY()
	
public:
	ACC_AI_PatrolPoint();

	bool bIsOccupied = false;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crash|AI|Components")
	class USphereComponent* EditorSphere;
#endif
};
