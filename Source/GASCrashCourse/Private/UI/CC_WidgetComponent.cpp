// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CC_WidgetComponent.h"
#include "Characters/CC_BaseCharacter.h"
#include "AbilitySystem/CC_AbilitySystemComponent.h"
#include "AbilitySystem/CC_AttributeSet.h"
#include "Blueprint/WidgetTree.h"
#include "UI/CC_AttributeWidget.h"

void UCC_WidgetComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAbilitySystemData();

	FString CharName = CrashCharacter->GetName();

	if (IsASCInitialized())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s] ASC is ready in BeginPlay. Initializing Delegate."), *CharName);
		InitializeAttributeDelegate();
	}
	else
	{
		UAbilitySystemComponent* ASC = CrashCharacter.IsValid() ? CrashCharacter->GetAbilitySystemComponent() : nullptr;
		UAttributeSet* AS = CrashCharacter.IsValid() ? CrashCharacter->GetAttributeSet() : nullptr;

		if (ASC && AS)
		{
			UE_LOG(LogTemp, Warning, TEXT("[%s] ASC found manually. Simulating OnASCInitialized."), *CharName);
			OnASCInitialized(ASC, AS);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("[%s] ASC NOT found. Binding to OnASCInitialized and waiting..."), *CharName);
			CrashCharacter->OnASCInitialized.AddDynamic(this, &ThisClass::OnASCInitialized);
		}
	}
}

void UCC_WidgetComponent::InitAbilitySystemData()
{
	CrashCharacter = Cast<ACC_BaseCharacter>(GetOwner());
	if (CrashCharacter.IsValid())
	{
		AttributeSet = Cast<UCC_AttributeSet>(CrashCharacter->GetAttributeSet());
		AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(CrashCharacter->GetAbilitySystemComponent());
	}
}

bool UCC_WidgetComponent::IsASCInitialized() const
{
	return AbilitySystemComponent.IsValid() && AttributeSet.IsValid();
}

void UCC_WidgetComponent::OnASCInitialized(UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	// 1. Know real names of classes used
	FString AscClassName = ASC ? ASC->GetClass()->GetName() : TEXT("NULL");
	FString AsClassName = AS ? AS->GetClass()->GetName() : TEXT("NULL");

	UE_LOG(LogTemp, Error, TEXT("--- DEBUG IN: ASC class is [%s] | AS class is [%s]"), *AscClassName, *AsClassName);

	AttributeSet = Cast<UCC_AttributeSet>(AS);
	AbilitySystemComponent = Cast<UCC_AbilitySystemComponent>(ASC);

	// 2. Check which Cast returned nullptr
	UE_LOG(LogTemp, Error, TEXT("--- DEBUG CAST: CC_ASC is %s | CC_AS is %s"), 
		AbilitySystemComponent.IsValid() ? TEXT("VALID") : TEXT("INVALID"),
		AttributeSet.IsValid() ? TEXT("VALID") : TEXT("INVALID"));

	if (!IsASCInitialized())
	{
		UE_LOG(LogTemp, Error, TEXT("--- ABORT: IsASCInitialized failed!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("OnASCInitialized 2"));
	InitializeAttributeDelegate();
}

void UCC_WidgetComponent::InitializeAttributeDelegate()
{
	if (!AttributeSet->bAttributesInitialized)
	{
		AttributeSet->OnAttributesInitialized.AddDynamic(this, &ThisClass::BindToAttributeChanges);
	}
	else
	{
		BindToAttributeChanges();
	}
}

void UCC_WidgetComponent::BindToAttributeChanges()
{
	for (const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair : AttributeMap)
	{
		BindWidgetToAttributeChanges(GetUserWidgetObject(), Pair); // For checking the owned widget object

		GetUserWidgetObject()->WidgetTree->ForEachWidget([this, Pair](UWidget* ChildWidget)
		{
			BindWidgetToAttributeChanges(ChildWidget, Pair);
		});
	}
}

void UCC_WidgetComponent::BindWidgetToAttributeChanges(UWidget* WidgetObject, const TTuple<FGameplayAttribute, FGameplayAttribute>& Pair) const
{
	if (!WidgetObject)
	{
		UE_LOG(LogTemp, Error, TEXT("BindWidgetToAttributeChanges: WidgetObject is NULL!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[%s] Checking widget: %s against Pair: %s"),
		*GetOwner()->GetName(),
		*WidgetObject->GetName(),
		*Pair.Key.AttributeName);

	UCC_AttributeWidget* AttributeWidget = Cast<UCC_AttributeWidget>(WidgetObject);
	if (!IsValid(AttributeWidget))
	{
		UE_LOG(LogTemp, Error, TEXT("--- FAILED: Widget %s is NOT a UCC_AttributeWidget!"), *WidgetObject->GetName());
		return; // We only care about CC Aattribute Widgets
	}
	if (!AttributeWidget->MatchesAttributes(Pair))
	{
		UE_LOG(LogTemp, Warning, TEXT("--- SKIPPED: Widget %s does not match this Attribute Pair."), *WidgetObject->GetName());
		return; // Only subscribe for matching Attributes
	}

	UE_LOG(LogTemp, Error, TEXT("+++ SUCCESS: Successfully bound Widget %s to %s"), *WidgetObject->GetName(), *Pair.Key.AttributeName);

	AttributeWidget->OnAttributeChange(Pair, AttributeSet.Get(), 0.f); // For initial values
	TWeakObjectPtr<UCC_AttributeWidget> WeakWidget(AttributeWidget);

	auto DelegateLambda = [this, WeakWidget, Pair](const FOnAttributeChangeData& AttributeChangeData)
		{
			if (WeakWidget.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Delegate fired for %s. New Value: %f"), *Pair.Key.AttributeName, AttributeChangeData.NewValue);
				WeakWidget->OnAttributeChange(Pair, AttributeSet.Get(), AttributeChangeData.OldValue);
			}
		};

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Key).AddLambda(DelegateLambda);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value).AddLambda(DelegateLambda);
}
