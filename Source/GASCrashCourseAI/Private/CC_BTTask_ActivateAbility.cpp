// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_BTTask_ActivateAbility.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UCC_BTTask_ActivateAbility::UCC_BTTask_ActivateAbility()
{
	NodeName = TEXT("Activate GAS Ability By Tag");
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UCC_BTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!IsValid(AIController)) return EBTNodeResult::Failed;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!IsValid(ControlledPawn)) return EBTNodeResult::Failed;

	CachedASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(ControlledPawn);
	if (!CachedASC.IsValid()) return EBTNodeResult::Failed;

	CachedOwnerComp = &OwnerComp;
	bAbilityFinishedInstantly = false;

	DelegateHandle = CachedASC->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnAbilityEnded);

	bool bSuccess = CachedASC->TryActivateAbilitiesByTag(AbilityTags);

	if (!bSuccess)
	{
		CachedASC->AbilityEndedCallbacks.Remove(DelegateHandle);
		return EBTNodeResult::Failed;
	}

	if (bAbilityFinishedInstantly)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task finished instantly!"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UCC_BTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (CachedASC.IsValid())
	{
		CachedASC->CancelAbilities(&AbilityTags);
	}

	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UCC_BTTask_ActivateAbility::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (CachedASC.IsValid() && DelegateHandle.IsValid())
	{
		CachedASC->AbilityEndedCallbacks.Remove(DelegateHandle);
		DelegateHandle.Reset();
	}

	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UCC_BTTask_ActivateAbility::OnAbilityEnded(UGameplayAbility* Ability)
{
	if (Ability && Ability->AbilityTags.HasAny(AbilityTags))
	{
		if (CachedOwnerComp.IsValid())
		{
			bAbilityFinishedInstantly = true;

			FinishLatentTask(*CachedOwnerComp.Get(), EBTNodeResult::Succeeded);
		}
	}
}
