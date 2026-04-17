// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Player/CC_Primary.h"
#include "Engine/OverlapResult.h"

void UCC_Primary::HitBoxOverlapTest()
{
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetAvatarActorFromActorInfo());

	// Ensure that the overlap test ignores the Avatar Actor
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActors(ActorsToIgnore);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetAllChannels(ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_Pawn, ECR_Block);

	TArray<FOverlapResult> OverlapResults;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(HitBoxRadius);

	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector() * HitBoxForwardOffset;
	const FVector HitBoxLocation = GetAvatarActorFromActorInfo()->GetActorLocation() + Forward + FVector(0, 0, HitBoxElevationOffset);

	GetWorld()->OverlapMultiByChannel(OverlapResults, HitBoxLocation, FQuat::Identity, ECC_Visibility, Sphere, QueryParams, ResponseParams);

	if (bDrawDebugs)
	{
		DrawDebugSphere(GetWorld(), HitBoxLocation, HitBoxRadius, 16, FColor::Blue, false, 3.f);

		for (const FOverlapResult& Result : OverlapResults)
		{
			if (IsValid(Result.GetActor()))
			{
				FVector DebugLocation = Result.GetActor()->GetActorLocation();
				DebugLocation.Z += 100.f;
				DrawDebugSphere(GetWorld(), DebugLocation, 30.f, 10, FColor::Black, false, 3.f);
			}
		}
	}
}
