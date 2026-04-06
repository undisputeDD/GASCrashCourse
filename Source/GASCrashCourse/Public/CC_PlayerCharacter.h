// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CC_BaseCharacter.h"
#include "CC_PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/**
 * 
 */
UCLASS()
class GASCRASHCOURSE_API ACC_PlayerCharacter : public ACC_BaseCharacter
{
	GENERATED_BODY()

public:
	ACC_PlayerCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Works on the server
	virtual void PossessedBy(AController* NewController) override;

	// Works on clients
	virtual void OnRep_PlayerState() override;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = "Camera")
	TObjectPtr<UCameraComponent> FollowCamera;
};
