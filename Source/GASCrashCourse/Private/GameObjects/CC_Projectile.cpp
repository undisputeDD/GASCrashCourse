// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/CC_Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Characters/CC_PlayerCharacter.h"
#include "AbilitySystemComponent.h"

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
	if (!IsValid(PlayerCharacter) || !PlayerCharacter->IsAlive()) return;

	UAbilitySystemComponent* AbilitySystemComponent = PlayerCharacter->GetAbilitySystemComponent();
	if (!IsValid(AbilitySystemComponent) || !HasAuthority()) return;

	FGameplayEffectContextHandle ContextHandle = AbilitySystemComponent->MakeEffectContext();
	FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DamageEffect, 1.f, ContextHandle);

	// TODO: Use the damage variable for the amount of damage to cause

	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
	Destroy();
}

