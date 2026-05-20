// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CC_Melee_Attack.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API UCC_Melee_Attack : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

private:
	TArray<FHitResult> PerformSphereTrace(USkeletalMeshComponent* MeshComp);

	void SendEventsToActors(USkeletalMeshComponent* MeshComp, const TArray<FHitResult>& Hits);

private:
	UPROPERTY(EditDefaultsOnly)
	FName SocketName;

	UPROPERTY(EditDefaultsOnly)
	float SocketExtensionOffset;
};
