// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "CC_EnemyAIController.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UBehaviorTree;
class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API ACC_EnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACC_EnemyAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* Actor, FAIStimulus Stimulus);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Crash|AI")
	TObjectPtr<UBehaviorTree> BehaviorTreeAsset;

private:
	void OnPawnDeath();
	void OnHealNeeded();
	
};
