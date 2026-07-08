// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/AnimNotify_Attack.h"
#include "Perception/AISense_Hearing.h"

void UAnimNotify_Attack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    UE_LOG(LogTemp, Display, TEXT("AnimNotifyAttack Started"));
    if (AActor* Owner = MeshComp->GetOwner())
    {
        UAISense_Hearing::ReportNoiseEvent(Owner->GetWorld(), Owner->GetActorLocation(), 0.5f, Owner, 0.0f, TEXT("Noise.Attack"));

        UE_LOG(LogTemp, Display, TEXT("AnimNotifyAttack After Report"));
    }
}
