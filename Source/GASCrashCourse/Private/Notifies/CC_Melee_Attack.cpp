// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CC_Melee_Attack.h"

void UCC_Melee_Attack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp) || !IsValid(MeshComp->GetOwner())) return;

	PerformSphereTrace(MeshComp);
}

TArray<FHitResult> UCC_Melee_Attack::PerformSphereTrace(USkeletalMeshComponent* MeshComp)
{
	FVector Start, End, ExtendedSocketDirection;
	float SphereTraceRadius = 25.0f;

	FTransform Transform = MeshComp->GetSocketTransform(SocketName);
	Start = Transform.GetLocation();
	ExtendedSocketDirection = Transform.GetRotation().GetForwardVector() * SocketExtensionOffset;
	End = Start - ExtendedSocketDirection;

	TArray<FHitResult> OutResults;
	FCollisionShape TraceSphere = FCollisionShape::MakeSphere(SphereTraceRadius);
	//FCollisionQueryParams QueryParams;
	//QueryParams.AddIgnoredActor(MeshComp->GetOwner());
	//GetWorld()->SweepMultiByChannel(OutResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, TraceSphere, QueryParams);
	GetWorld()->SweepMultiByChannel(OutResults, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, TraceSphere);

	return OutResults;
}

void UCC_Melee_Attack::SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits)
{
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
	}
}
