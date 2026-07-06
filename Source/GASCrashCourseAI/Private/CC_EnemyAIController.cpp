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
#include "Perception/AISenseConfig_Hearing.h"

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
	// SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	// SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	EnemyPerceptionComponent->ConfigureSense(*SightConfig);

	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

	HearingConfig->HearingRange = 3000.f;
	HearingConfig->SetMaxAge(5.f);

	HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	// HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	// HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

	EnemyPerceptionComponent->ConfigureSense(*HearingConfig);

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
		BlackboardComponent->InitializeBlackboard(*BehaviorTreeAsset->BlackboardAsset);

		if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(InPawn))
		{
			BlackboardComponent->SetValueAsFloat(FName("AcceptanceRadius"), Enemy->AcceptanceRadius - 100.f);
		}

		BehaviorTreeComponent->StartTree(*BehaviorTreeAsset);
	}

	if (ACC_EnemyCharacter* BaseChar = Cast<ACC_EnemyCharacter>(InPawn))
	{
		SightConfig->SightRadius = BaseChar->SearchRange;
		SightConfig->LoseSightRadius = BaseChar->SearchRange + 200.f;

		BaseChar->OnDeath.AddUObject(this, &ThisClass::OnPawnDeath);

		EnemyPerceptionComponent->ConfigureSense(*SightConfig);
	}
}

void ACC_EnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITOR
	if (ACC_EnemyCharacter* Enemy = Cast<ACC_EnemyCharacter>(GetPawn()))
	{
		FVector EyeLoc;
		FRotator EyeRot;
		Enemy->GetActorEyesViewPoint(EyeLoc, EyeRot);
		FVector LookDir = EyeRot.Vector();

		float HalfAngleRads = FMath::DegreesToRadians(SightConfig->PeripheralVisionAngleDegrees);

		DrawDebugCone(GetWorld(), EyeLoc, LookDir, SightConfig->SightRadius, HalfAngleRads, HalfAngleRads, 12, FColor::Green, false, -1.f, 0, 1.5f);

		DrawDebugSphere(GetWorld(), EyeLoc, SightConfig->LoseSightRadius, 16, FColor::Magenta, false, -1.f, 0, 1.f);
	}
#endif
}

void ACC_EnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus Stimulus)
{
	UBlackboardComponent* BB = GetBlackboardComponent();
	if (!IsValid(BB)) return;

	UE_LOG(LogTemp, Display, TEXT("OnTargetDetected"));
	if (Actor->ActorHasTag(TEXT("Player")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			BB->SetValueAsObject(FName("TargetActor"), Actor);
			BB->ClearValue(FName("LastKnownLocation"));

			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("I SEE YOU!"));
		}
		else
		{
			BB->ClearValue(FName("TargetActor"));
			BB->SetValueAsVector(FName("LastKnownLocation"), Actor->GetActorLocation());

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
