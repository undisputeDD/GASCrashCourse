// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "CC_BTTask_ActivateAbility.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSEAI_API UCC_BTTask_ActivateAbility : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCC_BTTask_ActivateAbility();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|GAS")
	FGameplayTagContainer AbilityTags;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:
	void OnAbilityEnded(UGameplayAbility* Ability);

	TWeakObjectPtr<UAbilitySystemComponent> CachedASC;
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	FDelegateHandle DelegateHandle;
	bool bAbilityFinishedInstantly;
};
