// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CC_PlayerController.h"
#include "EnhancedInputSubsystems.h"

void ACC_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	
	if (!IsValid(InputSubsystem)) return;
	
	for (UInputMappingContext* Context : UInputMappingContexts)
	{
		InputSubsystem->AddMappingContext(Context, 0);
	}


}

void ACC_PlayerController::Jump()
{
	
}

void ACC_PlayerController::StopJumping()
{
	
}

void ACC_PlayerController::Move(const FInputActionValue& Value)
{
	
}

void ACC_PlayerController::Look(const FInputActionValue& Value)
{
	
}
