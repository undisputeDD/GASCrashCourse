// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_CheckHealCooldown.h"
#include "AIController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UCC_CheckHealCooldown::UCC_CheckHealCooldown()
{
	NodeName = "Check GAS Cooldown";
}

bool UCC_CheckHealCooldown::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return false;

	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return false;

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Pawn);
	if (!ASC) return false;

	bool bIsOnCooldown = ASC->HasMatchingGameplayTag(CooldownTag);

	UE_LOG(LogTemp, Display, TEXT("Heal needed %s"), bIsOnCooldown ? TEXT("NO") : TEXT("YES"));

	return !bIsOnCooldown;
}
