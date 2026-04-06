// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CC_PlayerState.h"
#include "AbilitySystemComponent.h"

ACC_PlayerState::ACC_PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystemComponent");

	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}
