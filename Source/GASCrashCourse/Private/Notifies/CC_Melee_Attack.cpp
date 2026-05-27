// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CC_Melee_Attack.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffectTypes.h"
#include "Characters/CC_EnemyCharacter.h"
#include "Characters/CC_PlayerCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTags/CCTags.h"

void UCC_Melee_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

	TArray<FHitResult> Hits = PerformSphereTrace(MeshComp);
	SendEventsToActors(MeshComp, Hits);
}

void UCC_Melee_Attack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	AlreadyHitActors.Empty();
}

TArray<FHitResult> UCC_Melee_Attack::PerformSphereTrace(USkeletalMeshComponent* MeshComp) const
{
	FVector Start, End, ExtendedSocketDirection;

	FTransform Transform = MeshComp->GetSocketTransform(SocketName);
	Start = Transform.GetLocation();
	ExtendedSocketDirection = Transform.GetRotation().GetForwardVector() * SocketExtensionOffset;
	End = Start - ExtendedSocketDirection;

	TArray<FHitResult> OutResults;
	FCollisionShape TraceSphere = FCollisionShape::MakeSphere(SphereTraceRadius);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(MeshComp->GetOwner());
	MeshComp->GetWorld()->SweepMultiByChannel(OutResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, TraceSphere, QueryParams);
	//MeshComp->GetWorld()->SweepMultiByChannel(OutResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, TraceSphere);

	return OutResults;
}

void UCC_Melee_Attack::SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits)
{
	ACC_EnemyCharacter* EnemyCharacter = Cast<ACC_EnemyCharacter>(MeshComp->GetOwner());
	if (!IsValid(EnemyCharacter)) return;

	UAbilitySystemComponent* ASC = EnemyCharacter->GetAbilitySystemComponent();
	if (!IsValid(ASC)) return;

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();

		if (AlreadyHitActors.Contains(HitActor)) continue;

		AlreadyHitActors.Add(HitActor);

		ACC_PlayerCharacter* PlayerCharacter = Cast<ACC_PlayerCharacter>(HitActor);
		if (!IsValid(PlayerCharacter) || !PlayerCharacter->IsAlive()) continue;

		UAbilitySystemComponent* VictimASC = PlayerCharacter->GetAbilitySystemComponent();
		if (!IsValid(VictimASC)) continue;

		FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();

		ContextHandle.AddHitResult(Hit);

		FGameplayEventData Payload;
		Payload.Target = PlayerCharacter;
		Payload.ContextHandle = ContextHandle;
		Payload.Instigator = MeshComp->GetOwner();

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(EnemyCharacter, CCTags::Events::Enemy::MeleeHitTrace, Payload);
	}
}
