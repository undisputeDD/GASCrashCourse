// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_AI_PatrolPoint.h"
#include "Components/SphereComponent.h"

ACC_AI_PatrolPoint::ACC_AI_PatrolPoint()
{
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITORONLY_DATA
	EditorSphere = CreateDefaultSubobject<USphereComponent>(TEXT("EditorSphere"));
	EditorSphere->InitSphereRadius(50.0f);
	EditorSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EditorSphere->bHiddenInGame = true;
	RootComponent = EditorSphere;
#endif
}
