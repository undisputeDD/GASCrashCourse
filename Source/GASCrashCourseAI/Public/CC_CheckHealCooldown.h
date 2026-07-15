// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GameplayTagContainer.h"
#include "CC_CheckHealCooldown.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_CheckHealCooldown : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UCC_CheckHealCooldown();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

public:
	UPROPERTY(EditAnywhere, Category = "Crash|AI|GAS")
	FGameplayTag CooldownTag;
};
