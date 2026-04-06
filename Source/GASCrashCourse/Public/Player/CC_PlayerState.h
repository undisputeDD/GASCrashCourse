// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CC_PlayerState.generated.h"

class UAbilitySystemComponent;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API ACC_PlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ACC_PlayerState();

private:
	UPROPERTY(VisibleAnywhere, Category = "Crash|Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
