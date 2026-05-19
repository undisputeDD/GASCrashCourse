// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/CC_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/CC_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CCTags.h"

// Sets default values
ACC_Projectile::ACC_Projectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");

	bReplicates = true;
}

void ACC_Projectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(OtherActor);

	if (IsValid(PlayerCharacter) && PlayerCharacter->IsAlive())
	{
		UAbilitySystemComponent* TargetASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerCharacter);

		UAbilitySystemComponent* SourceASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(GetInstigator());

		if (IsValid(TargetASC) && IsValid(SourceASC) && HasAuthority())
		{
			FGameplayEffectContextHandle ContextHandle = SourceASC->MakeEffectContext();

			ContextHandle.AddInstigator(GetInstigator(), this);
			FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

			if (SpecHandle.IsValid())
			{
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, CCTags::SetByCaller::Projectile, Damage);

				TargetASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
			}
		}
	}

	SpawnImpactEffects();

	Destroy();
}

