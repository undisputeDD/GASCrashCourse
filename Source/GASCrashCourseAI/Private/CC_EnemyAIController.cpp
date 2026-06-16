// Fill out your copyright notice in the Description page of Project Settings.


#include "CC_EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/CC_EnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CC_AI_PatrolWorldSubsystem.h"
#include "CC_AI_PatrolPoint.h"
#include "Characters/CC_PlayerCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

ACC_EnemyAIController::ACC_EnemyAIController()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	SightConfig->SightRadius = 1000.f;
	SightConfig->LoseSightRadius = 1200.f;
	SightConfig->PeripheralVisionAngleDegrees = 60.f;
	SightConfig->SetMaxAge(5.f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	EnemyPerceptionComponent->ConfigureSense(*SightConfig);
	EnemyPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ACC_EnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyPerceptionComponent)
	{
		EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACC_EnemyAIController::OnTargetDetected);
	}
}

void ACC_EnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (BehaviorTreeAsset != nullptr)
	{
		if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(InPawn))
		{
			BlackboardComponent->SetValueAsFloat(FName("AcceptanceRadius"), Enemy->AcceptanceRadius);
		}

		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);
		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}

	if (ACC_EnemyCharacter* BaseChar = Cast<ACC_EnemyCharacter>(InPawn))
	{
		SightConfig->SightRadius = BaseChar->AcceptanceRadius;
		SightConfig->LoseSightRadius = BaseChar->AcceptanceRadius + 200.f;

		BaseChar->OnDeath.AddUObject(this, &ThisClass::OnPawnDeath);

		EnemyPerceptionComponent->ConfigureSense(*SightConfig);
	}
}

void ACC_EnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!IsValid(BB)) return;

	if (Actor->ActorHasTag(TEXT("Player")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(FName("TargetActor"), Actor);

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("I SEE YOU!"));
		}
		else
		{
			BB->ClearValue(FName("TargetActor"));

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("I LOST YOU!"));
		}
	}
}

void ACC_EnemyAIController::OnPawnDeath()
{
	UE_LOG(LogTemp, Error, TEXT("OnPawnDeath called!"));

	APawn* ControlledPawn = GetPawn();
	if (!IsValid(ControlledPawn)) return;

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(ControlledPawn))
	{
		if (UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent())
		{
			ASC->CancelAllAbilities();
		}
	}

	if (BrainComponent)
	{
		BrainComponent->StopLogic(TEXT("Dead"));
	}

	if (UCharacterMovementComponent* MoveComp = ControlledPawn->FindComponentByClass<UCharacterMovementComponent>())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}

	if (UBlackboardComponent* BB = GetBlackboardComponent())
	{
		if (ACC_AI_PatrolPoint* Point = Cast<ACC_AI_PatrolPoint>(BB->GetValueAsObject(FName("PatrolPoint"))))
		{
			if (UWorld* World = GetWorld())
			{
				if (UCC_AI_PatrolWorldSubsystem* Subsystem = World->GetSubsystem<UCC_AI_PatrolWorldSubsystem>())
				{
					Subsystem->ReleasePoint(Point);
				}
			}
		}
	}

	ControlledPawn->DetachFromControllerPendingDestroy();
}
