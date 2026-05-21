// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/CC_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/CC_PlayerCharacter.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayTags/CCTags.h"
#include "Utils/CC_BlueprintLibrary.h"

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

		if (IsValid(TargetASC) && HasAuthority())
		{
			FGameplayEventData Payload;
			Payload.Instigator = GetOwner();
			Payload.Target = PlayerCharacter;

			UCC_BlueprintLibrary::SendDamageEventToPlayer(PlayerCharacter, DamageEffect, Payload, CCTags::SetByCaller::Projectile, Damage);
		}
	}

	SpawnImpactEffects();

	Destroy();
}

