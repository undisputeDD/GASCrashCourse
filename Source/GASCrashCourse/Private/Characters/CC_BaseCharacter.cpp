// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CC_BaseCharacter.h"
#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

namespace CrashTags
{
	const FName Player = FName("Player");
}

ACC_BaseCharacter::ACC_BaseCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	// Tick and refresh bone transforms whether rendered or not - for bone updates on a dedicated server
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
}

void ACC_BaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, bAlive);
}

UAbilitySystemComponent* ACC_BaseCharacter::GetAbilitySystemComponent() const
{
	return nullptr;
}

UAttributeSet* ACC_BaseCharacter::GetAttributeSet() const
{
	return nullptr;
}

void ACC_BaseCharacter::GiveStartupAbilities()
{
	if (!IsValid(GetAbilitySystemComponent())) return;

	for (const auto& Ability : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(Ability);

		GetAbilitySystemComponent()->GiveAbility(AbilitySpec);
	}
}

void ACC_BaseCharacter::InitializeAttributes() const
{
	checkf(IsValid(InitializeAttributesEffect),	TEXT("InitializeAttributesEffect not set."));

	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(InitializeAttributesEffect, 1.f, ContextHandle);

	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

void ACC_BaseCharacter::OnHealthChange(const FOnAttributeChangeData& AttributeChangeData)
{
	if (AttributeChangeData.NewValue <= 0.f)
	{
		HandleDeath();
	}
}

void ACC_BaseCharacter::HandleDeath()
{
	bAlive = false;
}

void ACC_BaseCharacter::HandleRespawn()
{
	bAlive = true;
}

void ACC_BaseCharacter::ResetAttributes()
{
	checkf(IsValid(ResetAttributesEffect), TEXT("ResetAttributesEffect not set."));

	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		return;
	}

	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(ResetAttributesEffect, 1.f, ContextHandle);

	if (SpecHandle.IsValid())
	{
		ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	}
}

void ACC_BaseCharacter::PrintActiveAbilities()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("PrintActiveAbilities: ASC is null!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== CURRENTLY ACTIVE ABILITIES ==="));
	int32 ActiveCount = 0;

	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.IsActive())
		{
			FString AbilityName = Spec.Ability ? Spec.Ability->GetName() : TEXT("UnknownAbility");

			UE_LOG(LogTemp, Warning, TEXT(" -> Active: %s (Level: %d)"), *AbilityName, Spec.Level);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, FString::Printf(TEXT("ACTIVE: %s"), *AbilityName));
			}

			ActiveCount++;
		}
	}

	if (ActiveCount == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT(" -> No active abilities found."));
	}
	UE_LOG(LogTemp, Warning, TEXT("=================================="));
}