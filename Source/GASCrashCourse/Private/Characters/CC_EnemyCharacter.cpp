// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CC_EnemyCharacter.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "AIController.h"
#include "GameplayTags/CCTags.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ACC_EnemyCharacter::ACC_EnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilitySystemComponent = CreateDefaultSubobject<UCC_AbilitySystemComponent>("CC_AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	AttributeSet = CreateDefaultSubobject<UCC_AttributeSet>("AttributeSet");
}

void ACC_EnemyCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bIsBeingLaunched);
}

UAbilitySystemComponent* ACC_EnemyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UAttributeSet* ACC_EnemyCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

void ACC_EnemyCharacter::StopMovementUntilLanded()
{
	bIsBeingLaunched = true;
	AAIController* AIController = GetController<AAIController>();
	if (!IsValid(AIController)) return;
	AIController->StopMovement();

	if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
	{
		BB->SetValueAsBool(FName("bIsAirborne"), true);
	}

	if (IsValid(AbilitySystemComponent))
	{
		FGameplayTagContainer TagsToCancel;
		TagsToCancel.AddTag(CCTags::CCAbilities::Enemy::Attack);
		AbilitySystemComponent->CancelAbilities(&TagsToCancel);
	}

	if (!LandedDelegate.IsAlreadyBound(this, &ThisClass::EnableMovementOnLanded))
	{
		LandedDelegate.AddDynamic(this, &ThisClass::EnableMovementOnLanded);
	}
}

void ACC_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!IsValid(GetAbilitySystemComponent())) return;

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);
	OnASCInitialized.Broadcast(GetAbilitySystemComponent(), GetAttributeSet());

	if (!HasAuthority()) return;

	GiveStartupAbilities();
	InitializeAttributes();

	UCC_AttributeSet* CC_AttributeSet = Cast<UCC_AttributeSet>(GetAttributeSet());
	if (!IsValid(CC_AttributeSet)) return;

	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(CC_AttributeSet->GetHealthAttribute()).AddUObject(this, &ThisClass::OnHealthChange);
}

void ACC_EnemyCharacter::HandleDeath()
{
	Super::HandleDeath();

	OnDeath.Broadcast();
}

void ACC_EnemyCharacter::HandleRespawn()
{
	Super::HandleRespawn();
}

void ACC_EnemyCharacter::EnableMovementOnLanded(const FHitResult& Hit)
{
	bIsBeingLaunched = false;

	AAIController* AIController = GetController<AAIController>();
	if (IsValid(AIController))
	{
		if (UBlackboardComponent* BB = AIController->GetBlackboardComponent())
		{
			BB->SetValueAsBool(FName("bIsAirborne"), false);
		}
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(this, CCTags::Events::Enemy::EndAttack, FGameplayEventData());
	LandedDelegate.RemoveAll(this);
}

void ACC_EnemyCharacter::OnHealthChange(const FOnAttributeChangeData& AttributeChangeData)
{
	if (AttributeChangeData.NewValue <= 25.f && AttributeChangeData.NewValue > 0.f)
	{
		OnHealNeeded.Broadcast();
	}

	Super::OnHealthChange(AttributeChangeData);
}