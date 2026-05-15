// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Enemy/CC_SearchForTarget.h"
#include "Characters/CC_EnemyCharacter.h"
#include "AIController.h"
#include "GameplayTags/CCTags.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitDelay.h"
#include "Utils/CC_BlueprintLibrary.h"
#include "Tasks/AITask_MoveTo.h"
#include "AbilitySystemComponent.h"

UCC_SearchForTarget::UCC_SearchForTarget()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void UCC_SearchForTarget::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	OwningEnemy = Cast<ACC_EnemyCharacter>(GetAvatarActorFromActorInfo());
	if (!OwningEnemy.IsValid()) return;
	OwningAIController = Cast<AAIController>(OwningEnemy->GetController());
	if (!OwningAIController.IsValid()) return;

	StartSearch();

	WaitGameplayEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, CCTags::Events::Enemy::EndAttack);
	if (WaitGameplayEventTask)
	{
		WaitGameplayEventTask->EventReceived.AddDynamic(this, &UCC_SearchForTarget::EndAttackEventReceived);
		WaitGameplayEventTask->ReadyForActivation();
	}
}

void UCC_SearchForTarget::StartSearch()
{
	if (bDrawDebugs) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, FString::Printf(TEXT("UCC_SearchForTarget::StartSearch")));
	if (!OwningEnemy.IsValid()) return;

	if (SearchDelayTask)
	{
		SearchDelayTask->EndTask();
		SearchDelayTask = nullptr;
	}

	const float SearchDelay = FMath::RandRange(OwningEnemy->MinAttackDelay, OwningEnemy->MaxAttackDelay);
	SearchDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, SearchDelay);
	if (SearchDelayTask)
	{
		SearchDelayTask->OnFinish.AddDynamic(this, &UCC_SearchForTarget::Search);
		SearchDelayTask->ReadyForActivation();
	}
}

void UCC_SearchForTarget::EndAttackEventReceived(FGameplayEventData Payload)
{
	StartSearch();
}

void UCC_SearchForTarget::Search()
{
	const FVector SearchOrigin = GetAvatarActorFromActorInfo()->GetActorLocation();
	FClosestActorWithTagResult ClosestActorResult = UCC_BlueprintLibrary::FindClosestActorWithTag(GetWorld(), SearchOrigin, CrashTags::Player);

	TargetBaseCharacter = Cast<ACC_BaseCharacter>(ClosestActorResult.Actor);

	if (!TargetBaseCharacter.IsValid())
	{
		StartSearch();
		return;
	}
	if (TargetBaseCharacter->IsAlive())
	{
		MoveToTargetAndAttack();
	}
	else
	{
		StartSearch();
	}
}

void UCC_SearchForTarget::MoveToTargetAndAttack()
{
	if (!OwningEnemy.IsValid() || !OwningAIController.IsValid() || !TargetBaseCharacter.IsValid()) return;

	if (!OwningEnemy->IsAlive())
	{
		StartSearch();
		return;
	}

	MoveToLocationOrActorTask = UAITask_MoveTo::AIMoveTo(OwningAIController.Get(), FVector(), TargetBaseCharacter.Get(), OwningEnemy->AcceptanceRadius);
	if (MoveToLocationOrActorTask)
	{
		MoveToLocationOrActorTask->OnMoveTaskFinished.AddUObject(this, &ThisClass::AttackTarget);
		MoveToLocationOrActorTask->ConditionalPerformMove();
	}
}

void UCC_SearchForTarget::AttackTarget(TEnumAsByte<EPathFollowingResult::Type> Result, AAIController* Controller)
{
	if (Result != EPathFollowingResult::Success)
	{
		StartSearch();
		return;
	}

	OwningEnemy->RotateToTarget(TargetBaseCharacter.Get());
	
	if (AttackDelayTask)
	{
		AttackDelayTask->EndTask();
		AttackDelayTask = nullptr;
	}

	AttackDelayTask = UAbilityTask_WaitDelay::WaitDelay(this, OwningEnemy->GetTimelineLength());
	if (AttackDelayTask)
	{
		AttackDelayTask->OnFinish.AddDynamic(this, &UCC_SearchForTarget::Attack);
		AttackDelayTask->ReadyForActivation();
	}
}

void UCC_SearchForTarget::Attack()
{
	const FGameplayTag AttackTag = CCTags::CCAbilities::Enemy::Attack;

	GetAbilitySystemComponentFromActorInfo()->TryActivateAbilitiesByTag(AttackTag.GetSingleTagContainer());
}

void UCC_SearchForTarget::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (MoveToLocationOrActorTask && MoveToLocationOrActorTask->IsActive())
	{
		MoveToLocationOrActorTask->ExternalCancel();
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}