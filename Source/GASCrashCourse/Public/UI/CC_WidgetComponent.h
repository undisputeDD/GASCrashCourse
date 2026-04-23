// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "CC_WidgetComponent.generated.h"

class ACC_BaseCharacter;
class UCC_AbilitySystemComponent;
class UCC_AttributeSet;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API UCC_WidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;

private:
	void InitAbilitySystemData();

private:
	TWeakObjectPtr<ACC_BaseCharacter> CrashCharacter;
	TWeakObjectPtr<UCC_AbilitySystemComponent> AbilitySystemComponent;
	TWeakObjectPtr<UCC_AttributeSet> AttributeSet;
};
