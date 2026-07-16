// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/CC_BaseCharacter.h"
#include "CC_EnemyCharacter.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

DECLARE_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_MULTICAST_DELEGATE(FOnHealNeededDelegate);

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API ACC_EnemyCharacter : public ACC_BaseCharacter
{
	GENERATED_BODY()

public:
	ACC_EnemyCharacter();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	virtual UAttributeSet* GetAttributeSet() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	float AcceptanceRadius{ 500.f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	float MinAttackDelay{ .1f };
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	float MaxAttackDelay{ .5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crash|AI")
	bool bIsMelee{false};

	UFUNCTION(BlueprintImplementableEvent)
	float GetTimelineLength();

	void StopMovementUntilLanded();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool bIsBeingLaunched{ false };

	UPROPERTY(EditAnywhere, Category = "Crash|AI")
	float SearchRange{ 700.f };

	FOnDeathDelegate OnDeath;
	FOnHealNeededDelegate OnHealNeeded;

protected:
	void BeginPlay();

	virtual void HandleDeath() override;

	virtual void HandleRespawn() override;

	virtual void OnHealthChange(const FOnAttributeChangeData& AttributeChangeData) override;

private:
	UFUNCTION()
	void EnableMovementOnLanded(const FHitResult& Hit);

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;
};
