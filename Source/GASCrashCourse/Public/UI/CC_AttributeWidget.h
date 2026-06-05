// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "CC_AttributeWidget.generated.h"

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API UCC_AttributeWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	bool MatchesAttributes(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const;

	void OnAttributeChange(const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair, UCC_AttributeSet* AttributeSet, float OldValue);
		
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Attribute Change"))
	void BP_OnAttributeChange(float NewValue, float NewMaxValue, float OldValue);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|Attributes")
	FGameplayAttribute Attribute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|Attributes")
	FGameplayAttribute MaxAttribute;

	UPROPERTY(BlueprintReadOnly, Category = "Crash|Attributes")
	TWeakObjectPtr<AActor> AvatarActor;
};
