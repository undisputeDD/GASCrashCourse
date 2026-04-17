// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/CC_GameplayAbility.h"
#include "CC_Primary.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API UCC_Primary : public UCC_GameplayAbility
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "Crash|Abilities")
	void HitBoxOverlapTest();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
	float HitBoxRadius = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
	float HitBoxForwardOffset = 200.f;

	UPROPERTY(EditDefaultsOnly, Category = "Crash|Abilities")
	float HitBoxElevationOffset = 20.f;
};
