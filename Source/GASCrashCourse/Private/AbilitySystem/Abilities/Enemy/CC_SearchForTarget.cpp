// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/CC_SearchForTarget.h"
#include "Characters/CC_EnemyCharacter.h"
#include "AIController.h"
#include "GameplayTags/CCTags.h"

UCC_SearchForTarget::UCC_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UCC_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	OwningEnemy = Cast<ACC_EnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!OwningEnemy.IsValid()) return;
	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	if (!OwningAIController.IsValid()) return;

	StartSearch();

	UAbilityAsync_WaitGameplayEvent* WaitGameplayEventTask = UAbilityAsync_WaitGameplayEvent::WaitGameplayEventToActor(GetAvatarActorFromActorInfo(), CCTags::Events::Enemy::EndAttack);

	WaitGameplayEventTask->EventReceived.AddDynaminc(this, &UCC_SearchForTarget::EndAttackEventReceived);
}

void UCC_SearchForTarget::StartSearch()
{
	
}

void UCC_SearchForTarget::EndAttackEventReceived(FEventReceivedDelegate Payload)
{
	StartSearch();
}
