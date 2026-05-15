// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "CC_SearchForTarget.generated.h"

class ACC_EnemyCharacter;
class AAIController;

UCLASS()
class GASCRASHCOURSE_API UCC_SearchForTarget : public UCC_GameplayAbility
{
	GENERATED_BODY()

public:
	UCC_SearchForTarget();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	TWeakObjectPtr<ACC_EnemyCharacter> OwningEnemy;
	TWeakObjectPtr<AAIController> OwningAIController;

private:
	void StartSearch();

	UFUNCTION()
	void EndAttackEventReceived(FEventReceivedDelegate Payload);
};
